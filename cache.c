#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//assume all addresses are referring to words

//command line arguments
// 1. block size (number of words)
// 2. number of blocks
// 3. associativity (only direct mapped is required, i.e. 1)
// 4. Hit time (in cycles)
// 5. Miss time (int cycles)

int main(int argc, char ** argv) {
    int blockSize = atoi(argv[1]);
    int numBlocks = atoi(argv[2]);
    int assoc = atoi(argv[3]);
    int hitTime = atoi(argv[4]);
    int missTime = atoi(argv[5]);

    int offsetBits = ceil(log2(blockSize));
    int indexBits = ceil(log2(numBlocks));
    int tagBits = 31 - offsetBits - indexBits;


    //initialize cache
    int cache[numBlocks][assoc];
    //probably more here

    //read input file and parse the addresses
    

    return 0;
}