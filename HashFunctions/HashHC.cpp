#include "HashHC.h"
#include "Utilities.h"
#include <vector>

HashHC::HashHC(int w, int k, int TableSize, int vecSize, int probes)
:HashInterface(w, k, TableSize, vecSize, probes){
    h_mapping = new std::unordered_map<int, unsigned int>[k];
}

HashHC::~HashHC(){
    delete[] h_mapping;
}

unsigned int HashHC::Hash(const std::vector<int> &p){

	int i, h_i;
	unsigned int tag=0, bit=0;
	for(i=0; i<k; i++){
        tag = tag << 1;
        h_i = hVec[i]->hash(p);
        auto existing_tag = h_mapping[i].find(h_i);
        if( existing_tag == h_mapping[i].end() ){
            bit = randBit(h_i); // use the value of h_i(p) as a seed for coin tossing -> unique f_i(h_i)
            h_mapping[i][h_i] = bit;
        }else{
            bit = h_mapping[i][h_i];
        }
        tag = tag | bit;
    }

	return tag;
}

// Given bucket tag (index), returns a vector of all the closest vertices (all their bucket indices)
int *HashHC::HashIndex(unsigned int index, bool insertingMode){
    int *neighbors;
    if(insertingMode){
        neighbors = new int[1];
        neighbors[0] = index;
        return neighbors;
    }

    std::vector <int> neighborProbes;
    int distance = 1;
    
    while( (int)(neighborProbes.size()) < numProbes){
        MaskPermutations(distance, &neighborProbes, numProbes);
        distance++;
    }
    for (long unsigned int i=0; i<neighborProbes.size(); i++){
        neighborProbes[i] ^= index;
    }

    neighbors = new int[numProbes];
    int i=0;
    for(auto neighbor: neighborProbes){
        neighbors[i++] = neighbor;
    }
    return neighbors;
}

// Truncates to vector <masks> integers that can generate integers for with [remBits] Hamming distance
void HashHC::MaskPermutations(int remBits, std::vector<int> *masks, int threshold, int pos, int val){

    if ( (int)(masks->size()) == threshold)
        return;

    if(pos == k || remBits == 0){
        masks->push_back(val);
        return;
    }

    int set;

    set = 1 << pos;
    val &= set;
    MaskPermutations(remBits-1, masks, threshold, pos+1, val);

    set = ~set;
    val &= set;
    MaskPermutations(remBits, masks, threshold, pos+1, val);
}