#include "HashHC.h"
#include "Utilities.h"
#include <vector>

HashHC::HashHC(int w, int k, int TableSize, int vecSize)
:HashInterface(w, k, TableSize, vecSize){}

HashHC::~HashHC(){}

unsigned int HashHC::Hash(const std::vector<int> &p){

	int i, h_i;
	unsigned int tag=0, bit=0;
	for(i=0; i<k; i++){
        h_i = hVec[i]->hash(p);
        bit = randBit(h_i); // use the value of h_i(p) as a seed for coin tossing -> unique f_i(h_i)
        tag = tag | bit;
        tag << 1;
    }

	return tag;
}