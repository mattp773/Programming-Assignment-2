#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
//assume all addresses are referring to words

//command line arguments
// 1. block size (number of words)
// 2. number of blocks
// 3. associativity (only direct mapped is required, i.e. 1)
// 4. Hit time (in cycles)
// 5. Miss time (in cycles)

struct cacheLine {
    int valid;
    int tag;
};

int main(int argc, char ** argv) {
    srand(time(NULL));
    //all arguments are required for the program to run
    if(argc != 6) {
        printf("Usage: ./cache <block size in words> <number of blocks> <associativity> <hit time in cycles> <miss time in cycles>\n");
        return 1;
    }

    //read in all the arguments to the corresponding variables
    int blockSize = atoi(argv[1]);
    int numBlocks = atoi(argv[2]);
    int assoc = atoi(argv[3]);
    int hitTime = atoi(argv[4]);
    int missTime = atoi(argv[5]);

    //calculate the number of index and offset bits 
    // based on the number of blocks and the block size
    int indexBits = ceil(log2(numBlocks));
    int offsetBits;
    if(assoc == 1) {
        offsetBits = ceil(log2(blockSize));
    }
    else {
        offsetBits = ceil(log2(assoc));
    }
    //initialize cache
    //set all valid bit and tag bits to 0
    struct cacheLine cache[numBlocks/assoc][assoc];
    for(int i = 0; i < numBlocks/assoc; i++) {
        for(int j = 0; j < assoc; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
        }
    }

    //open the input file and check to make sure it opens
    FILE * fp = fopen("test.txt", "r");
    if(fp == NULL) {
        printf("Error opening input file\n");
        return 1;
    }
    //track the number of addresses read, the number of hits, and the number of misses
    int addressCount = 0;
    int numHits = 0;
    int numMisses = 0;

    //variables for the address read in and the index and tag parsed from the address
    int address;
    int index;
    int tag;
    int offset;
    int hit;
    int replaced;
    int randOffset;
    //read in the addresses until the end of file is reached
    while(fscanf(fp, "%x", &address) > 0) {
        hit = 0;
        replaced = 0;
        addressCount++;

        //parse the index and tag from the address
        index = ((address) / blockSize) % (numBlocks/assoc);
        tag = address >> (indexBits + offsetBits);
        offset = address & 1;
        printf("Address: %x, Index: %x, Tag: %x, Offset: %x\n", address, index, tag, offset);

        //check if the index has valid data
        //if not, increment the number of misses and set the tag and valid bit
        //if it does, check if the tag matches the tag in the cache
        //if the tag does not match, increment the number of misses and set the new tag
        //if the tag does match, increment the number of hits
        for(int i = 0; i < assoc; i++) {
            if(cache[index][i].valid == 1 && cache[index][i].tag == tag) {
                hit = 1;
                printf("Hit: %x\n", address);
            }
        }
        if(hit) {
            numHits++;
        }
        else {
            numMisses++;
            for(int i = 0; i < assoc; i++) {
                if(cache[index][i].valid == 0) {
                    printf("invalid cache line %d\n", i);
                    cache[index][i].valid = 1;
                    cache[index][i].tag = tag;
                    replaced = 1;
                    break;
                }
            }
            if(replaced == 0) {
                randOffset = rand() % assoc;
                printf("random offset: %d\n", randOffset);
                cache[index][randOffset].valid = 1;
                cache[index][randOffset].tag = tag;
            }
        }
        
    }

    //print the number of hits and misses
    printf("%d hits and %d misses\n", numHits, numMisses);

    //calculate and print the hit/miss rates and AMAT
    float hitRate = (numHits * 100) / (float)addressCount;
    float missRate = (numMisses * 100) / (float)addressCount;
    int AMAT = (hitTime) + ((missRate / 100) * missTime);
    printf("Hit rate: %.2f%%\nMiss rate: %.2f%%\nAMAT = %d cycles\n", hitRate, missRate, AMAT);
    return 0;
}