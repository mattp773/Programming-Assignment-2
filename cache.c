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

struct cacheLine {
    int valid;
    int tag;
};

int main(int argc, char ** argv) {

    if(argc != 6) {
        printf("Usage: ./cache <block size in words> <number of blocks> <associativity> <hit time in cycles> <miss time in cycles>\n");
        return 1;
    }

    int blockSize = atoi(argv[1]);
    int numBlocks = atoi(argv[2]);
    int assoc = atoi(argv[3]);
    int hitTime = atoi(argv[4]);
    int missTime = atoi(argv[5]);
    printf("number of blocks: %d\n", numBlocks);

    int indexBits = ceil(log2(numBlocks));
    int offsetBits = ceil(log2(blockSize));
    // printf("Offset bits: %d\n", offsetBits);


    //initialize cache
    struct cacheLine cache[numBlocks];
    for(int i = 0; i < numBlocks; i++) {
        for(int j = 0; j < assoc; j++) {
            cache[i].valid = 0;
            cache[i].tag = 0;
        }
    }

    //read input file and parse the addresses
    FILE * fp = fopen("input.txt", "r");
    if(fp == NULL) {
        printf("Error opening input file\n");
        return 1;
    }
    int address;

    int numHits = 0;
    int numMisses = 0;
    int index;
    int tag;

    while(fscanf(fp, "%x", &address) > 0) {
        index = (address / blockSize) % numBlocks;
        tag = address >> (indexBits + offsetBits);

        if(cache[index].valid == 0) {
            cache[index].valid = 1;
            cache[index].tag = tag;
            numMisses++;
            printf("Miss: %x, Tag: %d, Index: %d\n", address, tag, index);
        }
        else if(cache[index].valid == 1 && cache[index].tag != tag) {
            numMisses++;
            cache[index].tag = tag;
            printf("Miss: %x, Tag: %d, Index: %d\n", address, tag, index);
        }
        else if(cache[index].valid == 1 && cache[index].tag == tag) {
            printf("Hit: %x, Tag: %d, Index: %d\n", address, tag, index);
            numHits++;
        }
    }
    printf("%d hits and %d misses\n", numHits, numMisses);
    return 0;
}