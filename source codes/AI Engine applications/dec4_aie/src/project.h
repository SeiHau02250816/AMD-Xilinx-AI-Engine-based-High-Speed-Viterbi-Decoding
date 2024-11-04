#include <adf.h>
#include "adf/new_frontend/adf.h"
#include "adf/new_frontend/types.h"
#include "kernels.h"
#include "kernels/include.h"

using namespace adf;

class simpleGraph : public adf::graph {
private:
    kernel tConstructKernel;
    kernel feedForwardKernel;
    kernel tbKernel;

public:
    input_plio  in;
    output_plio out;
    output_gmio out2;

    shared_buffer<int32> tables;

    simpleGraph() {
        in  = input_plio::create(plio_32_bits, "data/input.txt");
        out = output_plio::create(plio_32_bits, "data/output.txt");
        out2 = output_gmio::create("gmioOut", 64, 1000);

        // Kernel creation
        tConstructKernel = kernel::create(constructTables);
        source(tConstructKernel) =  "kernels/table_construct.cc";
        runtime<ratio>(tConstructKernel) = 0.9;
        dimensions(tConstructKernel.out[0]) = {TABLE_LENGTH};

        feedForwardKernel = kernel::create(feedForward);
        source(feedForwardKernel) = "kernels/feed_forward.cc";
        runtime<ratio>(feedForwardKernel) = 0.9;
        dimensions(feedForwardKernel.in[0]) = {2};
        dimensions(feedForwardKernel.in[1]) = {TABLE_LENGTH};
        dimensions(feedForwardKernel.out[0]) = {NUMBER_OF_STATES * 2};    

        tbKernel = kernel::create(traceback);
        source(tbKernel) = "kernels/traceback.cc";
        runtime<ratio>(tbKernel) = 0.9;
        dimensions(tbKernel.in[0]) = {NUMBER_OF_STATES * 2};
        dimensions(tbKernel.in[1]) = {TABLE_LENGTH};
        dimensions(tbKernel.out[0]) = {1};    


        // 2-dimensional buffer containing tables in a memory tile
        // with 1 input port and 1 output port
        // table[0] -> nextstate_table
        // table[1] -> output_table
        // table[2] -> input_table
        tables = shared_buffer<int32>::create({TABLE_LENGTH}, 1, 1);
        num_buffers(tables) = 2;

        // Define the read and write access scheme to the shared buffers
        write_access(tables.in[0]) = tiling({
            .buffer_dimension = {TABLE_LENGTH},
            .tiling_dimension = {TABLE_LENGTH},
            .offset = {0},
        });

        read_access(tables.out[0]) = tiling({
            .buffer_dimension = {TABLE_LENGTH},
            .tiling_dimension = {TABLE_LENGTH},
            .offset = {0},
        });

        // Define connections
        connect<>(tConstructKernel.out[0], tables.in[0]);

        connect<>(in.out[0], feedForwardKernel.in[0]);
        connect<>(tables.out[0], feedForwardKernel.in[1]);
        
        connect<>(feedForwardKernel.out[0], tbKernel.in[0]);
        connect<>(tables.out[0], tbKernel.in[1]);
        connect<>(tbKernel.out[0], out.in[0]);
        connect<>(tbKernel.out[0], out2.in[0]);
    }
};
