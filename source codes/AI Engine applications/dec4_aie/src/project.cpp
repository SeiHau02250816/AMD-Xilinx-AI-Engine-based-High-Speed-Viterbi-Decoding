#include <adf.h>
#include <fstream>
#include "kernels.h"
#include "kernels/include.h"
#include "project.h"

using namespace adf;

#if defined(__AIESIM__) || defined(__X86SIM__) || defined(__ADF_FRONTEND__)

simpleGraph mygraph;

int main(void) {
    int32* doutRef = (int32*) GMIO::malloc(TOTAL_LEN * 4);

    mygraph.init();
    mygraph.run(TOTAL_LEN);
    mygraph.out2.aie2gm(doutRef, TOTAL_LEN * 4);

    printf("\n\nOutput:\n");
    for (int i = DEPTH_OF_TRELLIS - 1; i < TOTAL_LEN; i++) {
        printf("%d ", doutRef[i]);
    }
    printf("\n\n");
        
    mygraph.end();
    return 0;
}

#endif