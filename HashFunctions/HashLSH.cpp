#include "HashLSH.h"
#include "Utilities.h"

#define M 4294967291 // 2^32 - 5

HashLSH::HashLSH(int w, int k, int TableSize, int vecSize)
: HashInterface(w,k,TableSize,vecSize, 1){}

unsigned int HashLSH::Hash(const std::vector<int> &p){

	int i,r; // r: random natural number
	unsigned int res=0;
	for(i=0; i<k; i++){
		r = random_float();
		res += r*hVec[i]->hash(p);
		res = (unsigned int)(modulo((long int)res, (long int)M));
	}

	return res;
}

HashLSH::~HashLSH(){}