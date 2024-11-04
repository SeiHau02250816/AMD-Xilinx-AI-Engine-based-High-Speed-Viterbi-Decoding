#ifndef FUNCTION_INCLUDES_H
#define FUNCTION_INCLUDES_H

#include <adf.h>

#define K                   9
#define NUMBER_OF_STATES    256
#define m                   (K - 1)
#define k                   1
#define n                   2
/*
Little degradation in performance achieved by limiting trellis depth to K * 5 -- Interesting to experiment with smaller
values and measure the resulting degradation 
*/
#define DEPTH_OF_TRELLIS    (K * 5)
#define ES_OVR_NO           1.0
#define MSG_LEN             1000
#define CHN_LEN             (MSG_LEN + m) * 2
#define TOTAL_LEN           (MSG_LEN + m + DEPTH_OF_TRELLIS - 1)

#if K == 3
    const int g[n][K] = {
        {1, 1, 1},                          // 7
        {1, 0, 1}                           // 5
    };
#elif K == 5
    const int g[n][K] = {
        {1, 1, 1, 0, 1},                    // 35
        {1, 0, 0, 1, 1}                     // 23
    };
#elif K == 7
    const int g[n][K] = {
        {1, 1, 1, 1, 0, 0, 1},              // 171
        {1, 0, 1, 1, 0, 1, 1}               // 133
    };
#elif K == 9
    const int g[n][K] = {
        {1, 1, 1, 1, 0, 1, 0, 1, 1},        // 753
        {1, 0, 1, 1, 1, 0, 0, 0, 1}         // 561
    };
#endif

// extern function
extern "C" {
    void dec2bin(int32 d, int32 size, int32 b[]);
    int32 bin2dec(int32 b[], int32 size);
    int32 pow_int32(int32 data, int32 factor);
}

#endif
