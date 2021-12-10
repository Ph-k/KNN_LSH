#include "HashInterface.hpp"
#include "Utilities.h"

HashInterface::HashInterface(int w, int k, int TableSize, int vecSize, int probes)
:w(w), k(k), TableSize(TableSize), vecSize(vecSize), numProbes(probes) {
    float t; // randomly selected integer 
	std::vector<ItemType>* rVec; //randomly selected vector of size vecSize
	Hhash *Hhasing;

	for(int i=0; i<k; i++){
		t = random_float((float)0,(float)w); // randomly selected integer
		rVec = uniformD_vec(vecSize); //randomly selected vector of size vecSize
		Hhasing = new Hhash(rVec, t, w); 
		hVec.push_back(Hhasing);
	}
}

HashInterface::~HashInterface() {
    for(auto it: hVec)
		delete it;
}