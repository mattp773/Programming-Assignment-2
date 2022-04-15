// Programming Assignment 2 for ECE356
// Cache Simulator
// Matt Porter, Logan Scott, and Grant Ballard
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

    //initialize cache
    struct cacheLine cache[numBlocks];
    for(int i = 0; i < numBlocks; i++) {
        for(int j = 0; j < assoc; j++) {
            cache[i].valid = 0;
            cache[i].tag = 0;
        }
    }

    //open the input file and check to make sure it opens
    FILE * fp = fopen("input.txt", "r");
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

    //DIRECT-MAPPED
    if(assoc == 1){
        int offsetBits = ceil(log2(blockSize));
        int indexBits = ceil(log2(numBlocks));

        //read in the addresses until the end of file is reached
        while(fscanf(fp, "%x", &address) > 0) {
            addressCount++;

            //parse the index and tag from the address
            index = ((address >> offsetBits) / blockSize) % numBlocks;
            tag = address >> (indexBits + offsetBits);

            //check if the index has valid data
            //if not, increment the number of misses and set the tag and valid bit
            //if it does, check if the tag matches the tag in the cache
            //if the tag does not match, increment the number of misses and set the new tag
            //if the tag does match, increment the number of hits
            if(cache[index].valid == 0) {
                cache[index].valid = 1;
                cache[index].tag = tag;
                numMisses++;
            }
            else if(cache[index].valid == 1 && cache[index].tag != tag) {
                numMisses++;
                cache[index].tag = tag;
            }
            else if(cache[index].valid == 1 && cache[index].tag == tag) {
                numHits++;
            }
        }
    }

    //2-WAY SET ASSOCIATIVE
    else if(assoc == 2){
        int offsetBits = ceil(log2(blockSize));
        int indexBits = (log(numBlocks)/log(2))  - 1; //2^k cache entries & 2^j associativity therefore index is k-j bits (UNSURE whether j is 1 or 2 but i believe 1) 
        int set;
        while(fscanf(fp, "%x", &address) > 0) {
            addressCount++;
            //reparse index
            //reparse tag
            set = addressCount % assoc; //see top of page 418 in textbook ==> (block number) modulo (num sets)
            if(set == 1){
                //search thru all tags of set 1
            }
            else{
                //search thru all tags of set 2
            }

        }
    }

    //if incorrect associativity provided, close and exit
    else{
        printf("Associativity can only be direct-mapped (1) or 2-way set associative (2).\n");
        fclose(fp);
        return 2;
    }

    //close out file
    fclose(fp);

    //print the number of hits and misses
    printf("%d hits and %d misses\n", numHits, numMisses);
    
    //calculate and print the hit/miss rates and AMAT
    float hitRate = (numHits * 100) / (float)addressCount;
    float missRate = (numMisses * 100) / (float)addressCount;
    int AMAT = (hitTime) + (missRate * missTime);
    printf("Hit rate: %.2f%%\nMiss rate: %.2f%%\nAMAT = %d cycles\n", hitRate, missRate, AMAT);
    return 0;
}