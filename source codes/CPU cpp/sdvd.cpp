// Soft Decision Viterbi Decoder

#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <bits/stdc++.h>

#include "vdsim.h"

using namespace std;

#undef SLOWACS
#define FASTACS
#undef NORM
#define MAXMATRIX 128
// #define DEBUG

int nxt_stat(int cur_state, int input, vector<int>& mem);
void dec2bin(int d, int size, vector<int>& b);
int bin2dec(vector<int>& b, int size);
void init_quantizer(void);
void init_adaptive_quant(float es_ovr_n0);
int soft_quant(float channel_symbol);
int soft_metric(int data, int guess);

vector<int> quantizer_table(256);

void sdvd(float es_ovr_n0, long channel_len, vector<float>& input_data, vector<int>& decoder_output_matrix) {
    long t;                                                                         // Time
    vector<int> mem(K);                                                             // Input + conv. encoder shift registers
    vector<vector<int>> input_table(NUMBER_OF_STATES, vector<int> (NUMBER_OF_STATES));            // Maps current / next state to input
    vector<vector<int>> output_table(NUMBER_OF_STATES, vector<int> (2));                   // Gives conv. encoder output
    vector<vector<int>> nextstate_table(NUMBER_OF_STATES, vector<int> (2));                // For current state, gives next given input
    vector<vector<int>> accum_err_metric(NUMBER_OF_STATES, vector<int> (2));               // Accumulated error metrics
    vector<vector<int>> state_history(NUMBER_OF_STATES, vector<int> (K * 5 + 1));          // State history table
    vector<int> state_seq(K * 5 + 1);                                               // State sequence list                                                 

    vector<int> binary_output(2);                                                   // Vector to store binary encoder output
    vector<int> branch_output(2);                                                   // Vector to store trial encoder output

    int m, n, depth_of_trellis, stepping, branch_metric, sh_ptr,
        sh_col, x, xx, h, hh, next_state, last_stop;                                // Misc variables
    
    /*********************************************************************************************************************************************/
    // n is 2^1 = 2 for rate 1/2
    n = 2;

    // memory length, m = K - 1
    m = K - 1;

    /*
    Little degradation in performance achieved by limiting trellis depth to K * 5 -- Interesting to experiment with 
    smaller values and measure the resulting degradation.
    */
    depth_of_trellis = K * 5;

    // Initialize data structures
    for (int i=0; i < NUMBER_OF_STATES; i++) {
        for (int j=0; j < NUMBER_OF_STATES; j++) {
            input_table[i][j] = 0;
        }

        for (int j=0; j<n; j++) {
            nextstate_table[i][j] = 0;
            output_table[i][j] = 0;
        }

        for (int j=0; j <= depth_of_trellis; j++) {
            state_history[i][j] = 0;
        }

        // Initial accum_error_matric[x][0] = 0
        accum_err_metric[i][0] = 0;

        // By setting accum_error_metric[x][1] to MAX_INT, we don't need a flag
        accum_err_metric[i][1] = INT_MAX;
    }

    /*
    Generate the state transition matrix, output matrix, and input matrix 
    -- Input matrix 
       shows how FEC encoder bits lead to next state

    -- Next state matrix 
       shows next state given current state and input bit
    
    -- Output matrix 
       shows FEC encoder output bits given current presumed encoder state and encoder input bit.
       This will be compared to actual received sumbols to determine metric for corresponding branch of trellis.
    */
    for (int cur_state=0; cur_state < NUMBER_OF_STATES; cur_state++) {
        for (int in=0; in<n; in++) {
            next_state = nxt_stat(cur_state, in, mem);
            input_table[cur_state][next_state] = in;

            // Now compute the convolutional encoder output given the current state number and the input value
            branch_output[0] = 0;
            branch_output[1] = 0;

            for (int i=0; i<K; i++) {
                branch_output[0] ^= mem[i] & g[0][i];
                branch_output[1] ^= mem[i] & g[1][i];
            }

            // Next state, given current state and input
            nextstate_table[cur_state][in] = next_state;

            // Output in decimal, given current state and input
            output_table[cur_state][in] = bin2dec(branch_output, 2);
        }
    }

    #ifdef DEBUG
        cout << "\nInput:" << endl;
        for (int j=0; j<NUMBER_OF_STATES; j++) {
            for (int l=0; l<NUMBER_OF_STATES; l++) {
                cout << setprecision(4) << input_table[j][l] << " ";
            }
            cout << endl;
        }

        cout << "\nOutput:" << endl;
        for (int j=0; j<NUMBER_OF_STATES; j++) {
            for (int l=0; l<n; l++) {
                cout << setprecision(4) << output_table[j][l] << " ";
            }
            cout << endl;
        }

        cout << "\nNext State:" << endl;
        for (int j=0; j<NUMBER_OF_STATES; j++) {
            for (int l=0; l<n; l++) {
                cout << "Cur state: " << j << ", input: " << l << ", Next state: " << nextstate_table[j][l] << endl;
            }
            cout << endl;
        }
    #endif

    vector<int> decoder_input_matrix(channel_len);

    // Now we are going to rearrange the channel output so it has n rows, and n/2 columns where each row
    // corresponds to a channel symbol for a given bit and each column corresponds to an encoded bit.
    channel_len = channel_len / n;

    // Interesting to compare performace of fixed vs adaptive quantizer
    // init_quantizer();
    init_adaptive_quant(es_ovr_n0);

    // Quantize the channel output -- Convert float to short integer
    // Channel_output_matrix = reshape(channel_output, n, channel_length)
    for (t=0; t < (channel_len * n); t += n) {
        for (int i=0; i<n; i++) {
            decoder_input_matrix[t / n + i * channel_len] = soft_quant(input_data[t + i]);
        }
    }

    /*****************************************END OF SETUP**************************************************/
    // Start decoding channel outputs with forward traversal of trellis!
    // Parallel programming this part!
    for (t=0; t<channel_len - m; t++) {
        if (t <= m) {
            // Assume starting with zeroes, so just compute paths from all-zeros state
            stepping = pow(2, m - t);
        }
        else   
            stepping = 1;

        // We are going to use the state history array as a circular buffer so we don't have to shift the 
        // whole thing left after each bit is processed so that means we need an appropriate pointer.
        // Set up the state history array pointer for this time step t
        sh_ptr = (int) ((t + 1) % (depth_of_trellis + 1));

        // Repeat for each possible state
        for (int j=0; j < NUMBER_OF_STATES; j += stepping) {
            // Repeat for each possible convolutional encoder output n-tuple
            for (int l=0; l<n; l++) {
                branch_metric = 0;

                // Compute branch metric per channel symbol, and sum for all channel symbols in the 
                // convolutional encoder output n-tuple.
                #ifdef SLOWACS
                    // Convert the decimal representation of the encoder output to binary
                    dec2bin(output[j][l], n, binary_output);

                    // Compute branch metric per channel symbol, and sum for all channel symbols in 
                    // the convolutional encoder output n-tuple
                    for (int ll=0; ll<n; ll++) {
                        branch_metric = branch_metric + soft_metric(decoder_input_matrix[ll * channel_len + t], binary_output[ll]);
                    }
                #endif

                #ifdef FASTACS
                    // This part only works for n = 2, but it's fast!
                    // Convert the decimal representation of the encoder output to binary
                    binary_output[0] = (output_table[j][l] & 0x00000002) >> 1;
                    binary_output[1] = (output_table[j][l] & 0x00000001);

                    // Compute branch metric per channel symbol, and sum for all channel symbols in the convolutional encoder
                    // output n-tuple
                    branch_metric = branch_metric + abs(decoder_input_matrix[0 * channel_len + t] - 7 * binary_output[0]) +
                                    abs(decoder_input_matrix[1 * channel_len + t] - 7 * binary_output[1]);
                #endif

                // Now choose the surviving path -- The one with the smaller accumulated error matric!
                if (accum_err_metric[nextstate_table[j][l]][1] > accum_err_metric[j][0] + branch_metric) {
                    // Save an accumulated metric value for the survivor state
                    accum_err_metric[nextstate_table[j][l]][1] = accum_err_metric[j][0] + branch_metric;

                    // Update the state_history array with the state number of the survivor
                    state_history[nextstate_table[j][l]][sh_ptr] = j;
                }
            }
        }

        // For all rows of accum_err_metric, move col 2 to col 1 and flag col 2
        for (int j=0; j<NUMBER_OF_STATES; j++) {
            accum_err_metric[j][0] = accum_err_metric[j][1];
            accum_err_metric[j][1] = INT_MAX;
        }

        // Now start the traceback, if we've filled the trellis
        if (t >= depth_of_trellis - 1) {
            // Initialize the state sequence vector -- Probably unnecessary : FIXME
            for (int j=0; j<=depth_of_trellis; j++) {
                state_seq[j] = 0;
            }

            // Find the element of state_history with the minimum accum. error metric.
            // Since the outer states are reached by relatively-improbable runs of zeroes or ones,
            // search them from the top and bottom of the trellis in
            x = INT_MAX;

            for (int j=0; j < (NUMBER_OF_STATES / 2); j++) {
                if (accum_err_metric[j][0] < accum_err_metric[NUMBER_OF_STATES - 1 - j][0]) {
                    xx = accum_err_metric[j][0];
                    hh = j;
                }
                else {
                    xx = accum_err_metric[NUMBER_OF_STATES - 1 - j][0];
                    hh = NUMBER_OF_STATES - 1 - j;
                }

                if (xx < x) {
                    x = xx;
                    h = hh;
                }
            }

            #ifdef NORM
                // Interesting to experiment with different numbers of bits in the accumulated error metric -- 
                // Does performance decrease with fewer bits?

                // If the smallest accum. error is > MAXMETRIC, normalise the accum. error metrics by substracting the
                // value of the smallest one from all of them (making the smallest = 0) and saturate all other metrics
                // at MAXMETRIC
                if (x > MAXMETRIC) {
                    for (int j=0; j < NUMBER_OF_STATES; j++) {
                        accum_err_metric[j][0] = accum_err_metric[j][0] - x;

                        if (accum_err_metric[j][0] > MAXMETRIC) {
                            accum_err_metric[j][0] > MAXMETRIC;
                        }
                    }
                }
            #endif

            // Now pick the starting point for traceback
            state_seq[depth_of_trellis] = h;

            // Now work backwards from the end of the trellis to the oldest state in the trellis to determine
            // the optimal path. The purpose of this is to determine the most likely state sequence at the encoder
            // based on what channel symbols we received.
            for (int j=depth_of_trellis; j>0; j--) {
                sh_col = j + (sh_ptr - depth_of_trellis);
                if (sh_col < 0)
                    sh_col = sh_col + depth_of_trellis + 1;

                state_seq[j-1] = state_history[state_seq[j]][sh_col];
            }

            // Now figure out what  
            decoder_output_matrix[t - depth_of_trellis + 1] = input_table[state_seq[0]][state_seq[1]];
        }
    }

    /*******************************************************************************************************/
    // Now decode the encoder flushing channel-output bits
    for (int t = channel_len - m; t < channel_len; t++) {
        // Setup the state history array pointer for this time step t
        sh_ptr = (int)((t + 1) % (depth_of_trellis + 1));

        // Don't need to consider states where input was a 1, so determine what is the highest possible
        // state number where input was 0
        last_stop = NUMBER_OF_STATES / pow(2, t - channel_len + m);

        // Repeat for each possible state
        for (int j=0; j<last_stop; j++) {
            branch_metric = 0;
            dec2bin(output_table[j][0], n, binary_output);

            // Compute metric per channel bit, and sum for all channel bits in the convolutional encoder n-tuple
            for (int ll=0; ll<n; ll++) {
                branch_metric = branch_metric + soft_metric(decoder_input_matrix[ll * channel_len + t], binary_output[ll]);
            }

            // Now choose the surviving path -- the one with the smaller total metric
            if (accum_err_metric[nextstate_table[j][0]][1] > accum_err_metric[j][0] + branch_metric) {
                // Save a state metric value for the survivor state
                accum_err_metric[nextstate_table[j][0]][1] = accum_err_metric[j][0] + branch_metric;

                // Update the state_history array with the state number of the survivor
                state_history[nextstate_table[j][0]][sh_ptr] = j;
            }
        }

        // For all rows of accum_err_metric, swap columns 1 and 2
        for (int j=0; j<NUMBER_OF_STATES; j++) {
            accum_err_metric[j][0] = accum_err_metric[j][1];
            accum_err_metric[j][1] = INT_MAX;
        }

        // Now start the traceback if i >= depth of trellis - 1
        if (t >= depth_of_trellis - 1) {
            // Initialize the state sequence vector
            for (int j=0; j<=depth_of_trellis; j++) {
                state_seq[j] = 0;
            }

            // find the state_history element with the minimum accum. error metric
            x = accum_err_metric[0][0];
            h = 0;
            for (int j=1; j<last_stop; j++) {
                if (accum_err_metric[j][0] < x) {
                    x = accum_err_metric[j][0];
                    h = j;
                }
            }

            #ifdef NORM
                // If the smallest accum. error metric value is > MAXMETRIC, normalize the accum. 
                // error metrics by substracting the value of the smallest one from all of them
                // (making the smallest = 0) and saturate all other metrics at MAXMETRIC

                if (x > MAXMETRIC) {
                    for (int j=0; j<NUMBER_OF_STATES; j++) {
                        accum_err_metric[j][0] = accum_err_metric[j][0] - x;
                        if (accum_err_metric[j][0] > MAXMETRIC) {
                            accum_err_metric[j][0] = MAXMETRIC;
                        }
                    }
                }
            #endif

            state_seq[depth_of_trellis] = h;

            // Now work backwards from the end of the trellis to the oldest state in the trellis 
            // to determine the optimal path. The purpose of this is to determine the most likely 
            // state sequence at the encoder based on what channel symbols we received.
            for (int j = depth_of_trellis; j>0; j--) {
                sh_col = j + (sh_ptr - depth_of_trellis);
                if (sh_col < 0)
                    sh_col = sh_col + depth_of_trellis + 1;

                state_seq[j - 1] = state_history[state_seq[j]][sh_col];
            }

            // Now figure out what input sequence corresponds to the optimal path
            decoder_output_matrix[t - depth_of_trellis + 1] = input_table[state_seq[0]][state_seq[1]];
        }
    }

    for (int i=1; i < depth_of_trellis - m; i++) {
        decoder_output_matrix[channel_len - depth_of_trellis + i] = input_table[state_seq[i]][state_seq[i + 1]];
    }

    return;
}

/*
This function calculates the next state of the convolutional encoder, given the current state and input data.
It also calculates the memory contents of the convolutional encoder.
*/
int nxt_stat(int cur_state, int input, vector<int>& mem) {
    vector<int> binary_state(K - 1);            // Binary value of current state
    vector<int> next_state_binary(K - 1);       // Binary value of next_state
    int next_state;                             // Decimal value of next_state
    
    // Convert the decimal value of the current state number to binary
    dec2bin(cur_state, K - 1, binary_state);

    // Given the input and current state number, compute the next state number
    next_state_binary[0] = input;
    for (int i = 1; i < K - 1; i++) {
        next_state_binary[i] = binary_state[i-1];
    }

    // Convert the binary value of the next state number to decimal
    next_state = bin2dec(next_state_binary, K - 1);

    // Memory contents are the inputs to the modulo-two adders in the encoder
    mem[0] = input;
    for (int i=1; i<K; i++) {
        mem[i] = binary_state[i-1];
    }

    return next_state;
}

/*
This function converts a decimal number to a binary number, stored as a vector MSB first, having a 
specified number of bits with leading zeroes if necessary.
*/
void dec2bin(int d, int size, vector<int>& b) {
    for (int i=0; i<size; i++) {
        b[i] = 0;
    }

    b[size - 1] = d & 0x01;
    for (int i = size - 2; i>=0; i--) {
        d = d >> 1;
        b[i] = d & 0x01;
    }
}

/*
This function converts a binary number having a specified number of bits to the corresponding decimal number.
*/
int bin2dec(vector<int>& b, int size) {
    int d = 0;
    for (int i=0; i<size; i++) {
        d += (b[i] << (size - i - 1));
    }

    return d;
}

/*
This function initialize a 3-bit soft decision quantizer optimized for about 4 dB Eb/No
*/
void init_quantizer(void) {
    for (int i = -128; i < -31; i++) {
        quantizer_table[i + 128] = 7;
    }

    for (int i = -31; i < -21; i++) {
        quantizer_table[i + 128] = 6;
    }

    for (int i = -21; i < -11; i++) {
        quantizer_table[i + 128] = 5;
    }

    for (int i = -11; i < 0; i++) {
        quantizer_table[i + 128] = 4;
    }

    for (int i = 0; i < 11; i++) {
        quantizer_table[i + 128] = 3;
    }

    for (int i =11; i < 21; i++) {
        quantizer_table[i + 128] = 2;
    }

    for (int i = 21; i < 31; i++) {
        quantizer_table[i + 128] = 1;
    }

    for (int i = 31; i < 128; i++) {
        quantizer_table[i + 128] = 0;
    }
}

/*
This function initializes a quantizer that adapts to Es/No
*/
void init_adaptive_quant(float es_ovr_n0) {
    int d;
    float es, sn_ratio, sigma;

    es = 1;
    sn_ratio = (float) pow(10.0, (es_ovr_n0 / 10.0));
    sigma = (float) sqrt(es / (2.0 * sn_ratio));

    d = (int)(32 * 0.5 * sigma);

    for (int i = -128; i < (-3 * d); i++) {
        quantizer_table[i + 128] = 7;
    }

    for (int i = (-3 * d); i < (-2 * d); i++) {
        quantizer_table[i + 128] = 6;
    }

    for (int i = (-2 * d); i < (-1 * d); i++) {
        quantizer_table[i + 128] = 5;
    }

    for (int i = (-1 * d); i < 0; i++) {
        quantizer_table[i + 128] = 4;
    }

    for (int i = 0; i < (1 * d); i++) {
        quantizer_table[i + 128] = 3;
    }

    for (int i = (1 * d); i < (2 * d); i++) {
        quantizer_table[i + 128] = 2;
    }

    for (int i = (2 * d); i < (3 * d); i++) {
        quantizer_table[i + 128] = 1;
    }

    for (int i = (3 * d); i < 128; i++) {
        quantizer_table[i + 128] = 0;
    }
}

/*
This quantizer assumes that the mean channel_symbol value is +/- 1, and translates it to an integer
whose mean value is +/- 32 to address the lookup table "quantizer_table". 
Noted that overflow protection is included.
*/
int soft_quant(float channel_symbol) {
    int x = (int) (32 * channel_symbol);
    if (x < -128) x = -128;
    if (x > 127)  x = 127;

    return quantizer_table[x + 128];
}

/*
This metric is based on the algorithm given in Michelson and Levesque, page 323
*/
int soft_metric(int data, int guess) {
    return abs(data - (guess * 7));
}