#include <iostream>
#include <vector>

#include "vdsim.h"

using namespace std;

void cnvEnc(vector<vector<int>>& g, long input_len, vector<int>& in_array, vector<int>& out_array) {
    int m;                          /* K - 1 */
    long tt;                        /* Bit time, symbol time */
    vector<int> shift_reg(K, 0);    /* The encoder shift registers - Initialized to 0*/ 
    int sr_head;                    /* Index to the first elt in the shift registers */
    int p, q;                       /* The upper and lower xor gate outputs */

    m = K - 1;

    // Allocate space for zero-padded input data array
    vector<int> unencoded_data (input_len + m);

    // Read data and store it in the array
    for (int i=0; i < input_len; i++) {
        unencoded_data[i] = in_array[i];
    }

    // Zero-pad the end of data
    for (int i=0; i<m; i++) {
        unencoded_data[input_len + i] = 0;
    }

    /*
        To try speed things up a little, the shift register will be operated as a circular buffer, so it needs
        at least a head pointer. It doesn't need a tail pointer, though, since we won't be taking anything out 
        of it. We will just be overwriting the oldest entry with the new data    
    */

    sr_head = 0;

    // Initialize the channel symbol output index
    tt = 0;

    // Now start the encoding process
    // Compute the upper & lower mod-two adder outputs, one bit at a time.
    for (int i=0; i < input_len + m; i++) {
        shift_reg[sr_head] = unencoded_data[i];
        p = 0;
        q = 0;

        for (int j=0; j<K; j++) {
            int k = (j + sr_head) % K;
            p ^= shift_reg[k] & g[0][j];
            q ^= shift_reg[k] & g[1][j];
        }

        // Write the upper and lower xor gate outputs as channel symbols
        out_array[tt] = p;
        tt++;
        out_array[tt] = q;
        tt++;

        sr_head -= 1;
        if (sr_head < 0) {
            sr_head = m;
        }
    }
}