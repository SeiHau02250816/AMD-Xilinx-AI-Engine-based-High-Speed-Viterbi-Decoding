#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include <adf.h>
#include <climits>
#include "include.h"
#include "utils.cc"

int32 soft_quant(float channel_symbol);
int32 soft_metric(int32 data, int32 guess);
int32 pow_int32(int32 data, int32 factor);

static int32* quantizer_table;
static int32 t = 0;
static int32 accum_err_metric[NUMBER_OF_STATES][2];
static int32* nextstate_table;
static int32* output_table;
static int32 channel_len = CHN_LEN / n;

void feedForward(
    adf::input_buffer<float> & __restrict input_data, 
    adf::input_buffer<int32> & __restrict tables,
    adf::output_buffer<int32> & __restrict out
) {
    int32 stepping, branch_metric;
    int32 x, h;
    
    int32 decoder_input_matrix[n];
    int32 binary_output[2];
    

    int32* pOut = out.data();
    float* d = input_data.data();
    int32* state_history = out.data() + NUMBER_OF_STATES;

    if (t == 0) {
        nextstate_table = tables.data();
        output_table = tables.data() + (NUMBER_OF_STATES * 2);
        
        quantizer_table = tables.data() + (NUMBER_OF_STATES * 2 * 2);

        for (int i=0; i<NUMBER_OF_STATES; i++) {
            accum_err_metric[i][0] = 0;
            accum_err_metric[i][1] = INT_MAX;
        }
    }


    /**********************************************************************************************************************
    *************************************************FEED FORWARD**********************************************************    
    **********************************************************************************************************************/
    // Start decoding channel outputs with forward traversal of trellis!
    if (t < channel_len - m) {
        if (t < m) {
            // Assume starting with zeroes, so just compute paths from all-zero states
            stepping = pow_int32(2, m - t);
        }
        else {
            stepping = 1;
        }

        decoder_input_matrix[0] = soft_quant(d[0]);
        decoder_input_matrix[1] = soft_quant(d[1]);

        // We are going to use the state history array as a circular buffer so we don't have to shift the 
        // whole thing left after each bit is processed so that means we need an appropriate pointer.
        
        // Repeat for each possible state
        for (int32 j=0; j < NUMBER_OF_STATES; j += stepping) 
        chess_prepare_for_pipelining
        {
            // Repeat for each possible convolutional encoder output n-tuple
            for (int32 cin=0; cin<n; cin++) {
                branch_metric = 0;

                // This part only works for n = 2, but it's fast!
                // Convert the decimal representation of the encoder output to binary
                binary_output[0] = (output_table[j * n + cin] & 0x00000002) >> 1;
                binary_output[1] = (output_table[j * n + cin] & 0x00000001);
                
                // Compute branch metric per channel symbol, and sum for all channel symbols in the 
                // convolutional encoder output n-tuple
                branch_metric = branch_metric + abs(decoder_input_matrix[0] - 7 * binary_output[0]) +
                                abs(decoder_input_matrix[1] - 7 * binary_output[1]);
                
                // Now choose the surviving path -- The one with the smaller accumulated error metric!
                if (accum_err_metric[nextstate_table[j * n + cin]][1] > accum_err_metric[j][0] + branch_metric) {
                    // Save an accumulated metric value for the survivor state
                    accum_err_metric[nextstate_table[j * n + cin]][1] = accum_err_metric[j][0] + branch_metric;

                    // Update the state_history array with the state number of the survivor
                    state_history[nextstate_table[j * n + cin]] = j;
                }
            }
        }

        // For all rows of accum_error_metric, move col 2 to col 1 and flag col 2
        for (int j=0; j<NUMBER_OF_STATES; j++) 
        chess_prepare_for_pipelining
        {
            accum_err_metric[j][0] = accum_err_metric[j][1];
            pOut[j] = accum_err_metric[j][0];

            accum_err_metric[j][1] = INT_MAX;
        }
    }

    // Now decode the encoder flushing channel-output bits
    else if (t < channel_len) {
    // for (int32 t = channel_len - m; t < channel_len; t++) {
        decoder_input_matrix[0] = soft_quant(d[0]);
        decoder_input_matrix[1] = soft_quant(d[1]);

        // Don't need to consider states where input was a 1, so determine what is the highest possible
        // state number where input was 0
        int32 last_stop = NUMBER_OF_STATES / pow_int32(2, t - channel_len + m);

        // Repeat for each possible state
        for (int32 j=0; j<last_stop; j++) 
        chess_prepare_for_pipelining
        {
            branch_metric = 0;
            dec2bin(output_table[j * n], n, binary_output);

            // Compute metric per channel bit, and sum for all channel bits in the convolutional encoder n-tuple
            for (int32 ll=0; ll<n; ll++) {
                branch_metric = branch_metric + soft_metric(decoder_input_matrix[ll], binary_output[ll]);
            }

            // Now choose the surviving path -- the one with the smaller total metric
            if (accum_err_metric[nextstate_table[j * n]][1] > accum_err_metric[j][0] + branch_metric) {
                // Save an accumulated metric value for the survivor state
                accum_err_metric[nextstate_table[j * n]][1] = accum_err_metric[j][0] + branch_metric;

                // Update the state_history array with the state number of the survivor
                state_history[nextstate_table[j * n]] = j;
            }
        }

        // For all rows of accum_err_metric, swap columns 1 and 2
        for (int32 j=0; j<NUMBER_OF_STATES; j++) 
        chess_prepare_for_pipelining
        {
            accum_err_metric[j][0] = accum_err_metric[j][1];
            pOut[j] = accum_err_metric[j][0];
            
            accum_err_metric[j][1] = INT_MAX;
        }
    }

    t++;
}

/*
This quantizer assumes that the mean channel symbol value is +/- 1, and translates it to an integer
whose mean value is +/- 32 to address the lookup table "quantizer_table".
Noted that overflow protection is included.
*/
int32 soft_quant(float channel_symbol) {
    int32 x = aie::to_fixed(32.0 * channel_symbol);
    if (x < -128) x = -128;
    if (x > 127)  x = 127;

    return quantizer_table[x + 128];
}

/*
This metric is based on the algorithm given in Michelson and Levesque, page 323
*/
int32 soft_metric(int32 data, int32 guess) {
    return abs(data - (guess * 7));
}