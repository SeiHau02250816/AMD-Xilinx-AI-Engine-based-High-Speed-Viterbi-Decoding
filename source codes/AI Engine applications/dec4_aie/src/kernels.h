#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H
    #include "kernels/include.h"

    #define TABLE_LENGTH    ((NUMBER_OF_STATES * 2 * 2) + 256)

    void constructTables(adf::output_buffer<int32> & __restrict out);
    
    void feedForward(
        adf::input_buffer<float> & __restrict input_data, 
        adf::input_buffer<int32> & __restrict tables,
        adf::output_buffer<int32> & __restrict out
    );

    void traceback(
        adf::input_buffer<int32> & __restrict input_data, 
        adf::input_buffer<int32> & __restrict tables,
        adf::output_buffer<int32> & __restrict decoder_output_matrix
    );

#endif
