#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <adf.h>
#include "include.h"
#include "utils.cc"

int32 nxt_state(int32 cur_state, int32 input, int32 mem[]);
void init_adaptive_quantizer(int32* quantizer_table, float es_ovr_n0);

static int32 initialized = 0;

void constructTables(adf::output_buffer<int32> & __restrict out) {
    if (initialized == 0) {
        int32* pOut = out.data();
        int32 mem[K];
        int32 next_state;
        int32 branch_output[n];
        int32* quantizer_table = out.data() + (NUMBER_OF_STATES * 2 * 2);

        /*
        Generate the state transition matrix, output matrix and input matrix
        -- Input matrix
            Shows how FEC encoder bits lead to next state

        -- Next state matrix
            Shows next state given current state and input bit

        -- Output matrix
            Shows FEC encoder output bits given current presumed encoder state and encoder input bit.
            This will be compared to actual received symbols to determine metric for corresponding branch of trellis.
        */
        for (int cur_state = 0; cur_state < NUMBER_OF_STATES; cur_state++) 
        chess_prepare_for_pipelining
        {
            for (int in = 0; in < n; in++) {
                next_state = nxt_state(cur_state, in, mem);
                
                // Now compute the convolutional encoder output given the current state number and the input value
                branch_output[0] = 0;
                branch_output[1] = 0;

                for (int i=0; i<K; i++) {
                    branch_output[0] ^= mem[i] & g[0][i];
                    branch_output[1] ^= mem[i] & g[1][i];
                }

                // Next state, given current state and input
                pOut[(0 * NUMBER_OF_STATES * 2) + cur_state * n + in] = next_state; // Next state table

                // Output in decimal, given current state and input
                pOut[(1 * NUMBER_OF_STATES * 2) + cur_state * n + in] = bin2dec(branch_output, n); // Output table
            }
        }

        init_adaptive_quantizer(quantizer_table, ES_OVR_NO);

        initialized = 1;
    }
}

/*
This function calculates the next state of the convolutional encoder, given the current state and input data.
It also calculates the memory contents of the convolutional encoder
*/
int32 nxt_state(int32 cur_state, int32 input, int32 mem[]) {
    int32 binary_state[K - 1];          // Binary value of current state
    int32 next_state_binary[K - 1];     // Binary value of next state
    int32 next_state;                   // Decimal value of next_state

    // Convert the decimal value of the current state number to binary
    dec2bin(cur_state, K - 1, binary_state);

    // Given the input and current state number, compute the next state number
    next_state_binary[0] = input;
    for (int i=1; i < K - 1; i++) {
        next_state_binary[i] = binary_state[i - 1];
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
This function initialize a 3-bit soft decision quantizer optimized for about 4 dB Eb/No
*/
void init_adaptive_quantizer(int32* quantizer_table, float es_ovr_n0) {
    int32 d;
    float es, sn_ratio, sigma;

    es = 1.0;
    sn_ratio = (float) pow(10.0, (es_ovr_n0 / 10.0));
    sigma = (float) sqrt(es / (2.0 * sn_ratio));
    d = (int)(32 * 0.5 * sigma);

    for (int i = -128; i < (-3 * d); i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 7;
    }

    for (int i = (-3 * d); i < (-2 * d); i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 6;
    }

    for (int i = (-2 * d); i < (-1 * d); i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 5;
    }

    for (int i = (-1 * d); i < 0; i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 4;
    }

    for (int i = 0; i < (1 * d); i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 3;
    }

    for (int i = (1 * d); i < (2 * d); i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 2;
    }

    for (int i = (2 * d); i < (3 * d); i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 1;
    }

    for (int i = (3 * d); i < 128; i++) 
    chess_prepare_for_pipelining
    {
        quantizer_table[i + 128] = 0;
    }
}
