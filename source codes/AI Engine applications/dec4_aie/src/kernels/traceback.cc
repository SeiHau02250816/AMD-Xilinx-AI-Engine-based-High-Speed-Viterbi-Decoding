#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include <adf.h>
#include <climits>
#include "include.h"
#include "utils.cc"

static int32 t = 0;
static int32 state_seq[K * 5 + 1];
static int32 state_history_table[NUMBER_OF_STATES][K * 5 + 1];
static int32 channel_len = CHN_LEN / n;

int32 get_input(int32 cur_state, int32 next_state);

void traceback(
    adf::input_buffer<int32> & __restrict input_data, 
    adf::input_buffer<int32> & __restrict tables,
    adf::output_buffer<int32> & __restrict decoder_output_matrix
) {
    int32 sh_ptr, sh_col;
    int32 x, xx, h, hh;

    int32* accum_err_metric = input_data.data();
    int32* state_history_entry = input_data.data() + NUMBER_OF_STATES;
    int32* pOut = decoder_output_matrix.data();

    // Update state_history_table
    if (t < channel_len) {
        sh_ptr = (int32)((t + 1) % (DEPTH_OF_TRELLIS + 1));

        for (int32 i=0; i<NUMBER_OF_STATES; i++) {
            state_history_table[i][sh_ptr] = state_history_entry[i];
        }
    }
    
    /**********************************************************************************************************************
    ***************************************************TRACEBACK***********************************************************
    **********************************************************************************************************************/
    if (t < channel_len - m) {
        if (t >= DEPTH_OF_TRELLIS - 1) {
            // Initialize the state_seq vector
            for (int32 j=0; j<=DEPTH_OF_TRELLIS; j++) {
                state_seq[j] = 0;
            }

            // Find the element of state_history_table with the minimum accum. error metric.
            // Since the outer states are reached by relatively-improbable runs of zeroes and ones,
            // search them from the top and bottom of the trellis
            x = INT_MAX;

            for (int j=0; j < NUMBER_OF_STATES / 2; j++) {
                if (accum_err_metric[j] < accum_err_metric[NUMBER_OF_STATES - 1 - j]) {
                    xx = accum_err_metric[j];
                    hh = j;
                }
                else {
                    xx = accum_err_metric[NUMBER_OF_STATES - 1 - j];
                    hh = NUMBER_OF_STATES - 1 - j;
                }

                if (xx < x) {
                    x = xx;
                    h = hh;
                }
            }

            // Now pick the starting point for traceback
            state_seq[DEPTH_OF_TRELLIS] = h;

            // Now work backwards from the end of the trellis to the oldest state in the trellis to determine
            // the optimal path. The purpose of this is to determine the most likely state sequence at the encoder
            // based on what channel symbols we received
            for (int j = DEPTH_OF_TRELLIS; j>0; j--) {
                sh_col = j + (sh_ptr - DEPTH_OF_TRELLIS);
                if (sh_col < 0)
                    sh_col = sh_col + DEPTH_OF_TRELLIS + 1;

                state_seq[j-1] = state_history_table[state_seq[j]][sh_col];
            }

            // Now figure out what input sequence corresponds to the state sequence in the optimal path
            pOut[0] = get_input(state_seq[0], state_seq[1]);
        }
    }
    else if (t < channel_len) {
        if (t >= DEPTH_OF_TRELLIS - 1) {
            int32 last_stop = NUMBER_OF_STATES / pow_int32(2, t - channel_len + m);

            // Find the state_history_table element with the minimum accum. error metric
            x = accum_err_metric[0];
            h = 0;

            for (int j=1; j<last_stop; j++) {
                if (accum_err_metric[j] < x) {
                    x = accum_err_metric[j];
                    h = j;
                }
            }

            state_seq[DEPTH_OF_TRELLIS] = h;

            // Now work backwards from the end of the trellis to the oldest state in the trellis
            // to determine the optimal path. The purpose of this is to determine the most likely
            // state sequence at the encoder based on what channel symbols we received
            for (int32 j = DEPTH_OF_TRELLIS; j>0; j--) {
                sh_col = j + (sh_ptr - DEPTH_OF_TRELLIS);
                if (sh_col < 0)
                    sh_col = sh_col + DEPTH_OF_TRELLIS + 1;

                state_seq[j-1] = state_history_table[state_seq[j]][sh_col];
            }

            // Now figure out what input sequence corresponds to the optimal path
            pOut[0] = get_input(state_seq[0], state_seq[1]);
        }
    }
    else {
        int32 i = t - channel_len + 1;
        pOut[0] = get_input(state_seq[i], state_seq[i + 1]);
    }

    t++;
}

int32 get_input(int32 cur_state, int32 next_state) {
    int32 input;
    if (cur_state < next_state) {
        input = 1;
    }
    else {
        input = 0;
    }

    return input;
}