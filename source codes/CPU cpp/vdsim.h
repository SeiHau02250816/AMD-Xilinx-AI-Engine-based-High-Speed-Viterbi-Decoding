#ifndef VITERBI
#define VITERBI

#include <iostream>
#include <vector>

using namespace std;

#define K                   9               /* constraint length */
#define NUMBER_OF_STATES    256             /* 2 ^ (K - 1) -- CHANGE as required */
#define PI                  3.141592654     /* Circumference of circle divided by diameter */

#define MSG_LEN             100000l         /* How many bits in each test message */
#define DOENC                               /* Test with convolutional encoding/Viterbi Decoding */
#undef  DONOENC                             /* Test with no encoding */
#define LOESN0              0.0             /* Minimum Es/No at which to test */
#define HIESN0              1.0             /* Maximum Es/No at which to test */
#define ESN0STEP            0.5             /* Es/No increment for test driver */

#if K == 3
    vector<vector<int>> g = {
        {1, 1, 1},                          // 7
        {1, 0, 1}                           // 5
    };
#endif

#if K == 5
    vector<vector<int>> g = {
        {1, 1, 1, 0, 1},                    // 35
        {1, 0, 0, 1, 1}                     // 23
    };
#endif

#if K == 7
    vector<vector<int>> g = {
        {1, 1, 1, 1, 0, 0, 1},              // 171
        {1, 0, 1, 1, 0, 1, 1}               // 133
    };
#endif

#if K == 9
    vector<vector<int>> g = {
        {1, 1, 1, 1, 0, 1, 0, 1, 1},        // 753
        {1, 0, 1, 1, 1, 0, 0, 0, 1}         // 561
    };
#endif

void genData01(long len, vector<int>& out_array);
void cnvEnc(vector<vector<int>>& g, long input_len, vector<int>& in_array, vector<int>& out_array);
void addnoise(float es_ovr_n0, long channel_len, vector<int>& in_array, vector<float>& out_array);
void sdvd(float es_ovr_n0, long channel_len, vector<float>& channel_output_vector, vector<int>& decoder_output_matrix);

#endif