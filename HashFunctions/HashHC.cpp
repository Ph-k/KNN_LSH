#include "HashHC.h"
#include "Utilities.h"
#include <vector>

HashHC::HashHC(int w, int k, int TableSize, int vecSize)
:HashInterface(w, k, TableSize, vecSize){
    h_mapping = new std::unordered_map<int, unsigned int>[k];
}

HashHC::~HashHC(){
    delete[] h_mapping;
}

unsigned int HashHC::Hash(const std::vector<int> &p){

	int i, h_i;
	unsigned int tag=0, bit=0;
	for(i=0; i<k; i++){
        h_i = hVec[i]->hash(p);
        auto existing_tag = h_mapping[i].find(h_i);
        if( existing_tag == h_mapping[i].end() ){
            bit = randBit(h_i); // use the value of h_i(p) as a seed for coin tossing -> unique f_i(h_i)
            h_mapping[i][h_i] = bit;
        }else{
            bit = h_mapping[i][h_i];
        }
        tag = tag | bit;
        tag << 1;
    }

	return tag;
}