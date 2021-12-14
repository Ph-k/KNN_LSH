#include <vector>
#include "HashLSH.h"
#include "HashDF.h"
#include "../FredLib/FredLibWrapper.h"

#define M 4294967291 // 2^32 - 5

HashLSH::HashLSH(int w, int k, int TableSize, int vecSize, char method, double delta)
: HashInterface(w,k,TableSize,vecSize, 1), method(method), discreteFrechet(nullptr){
	switch (method){
		case __FRECHET_DISCRETE_MODE:
			discreteFrechet = new HashDF(delta, vecSize);
			break;
		default:
			break;
	}
}

unsigned int HashLSH::Hash(const std::vector<__TIMESERIES_X_TYPE> &p){

	int i,r; // r: random natural number
	unsigned int res=0;
	std::vector<__TIMESERIES_X_TYPE> *q_vec = nullptr;
	for(i=0; i<k; i++){
		r = random_float();

		switch (method){
			case __LSH_MODE:
				res += r*hVec[i]->hash(p);
				break;
			case __FRECHET_DISCRETE_MODE:
				q_vec = discreteFrechet->Snap(p);
				res += r*hVec[i]->hash(*q_vec);
				break;
			case __FRECHET_CONTINUOUS_MODE:
				q_vec = Simplify(p);
				res += r*hVec[i]->hash(*q_vec);
				break;
			default:
				std::cerr << "Error in HashLSH::Hash, invalid method!" << std::endl;
				break;
		}
		if(q_vec != nullptr) delete q_vec;

		res = (unsigned int)(modulo((long int)res, (long int)M));
	}

	return res;
}

int *HashLSH::HashIndex(unsigned int ID, bool insertingMode){
	int *bID = new int[1];
	bID[0] = ID%TableSize;
	return bID;
};

HashLSH::~HashLSH(){
	if(discreteFrechet != nullptr) delete discreteFrechet;
}