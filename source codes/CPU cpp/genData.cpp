#include <iostream>
#include <vector>
#include <cstdlib>

#include "vdsim.h"

using namespace std;

void genData01(long len, vector<int>& out_array) {
    // Re-seed the random number generator
    srand(0);

    for (long i=0; i<len; i++) {
        out_array[i] = (int)(rand() % 2);
    }
}