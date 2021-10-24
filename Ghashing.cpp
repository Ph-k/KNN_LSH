#include "Ghashing.h"
#define M 50000

Ghash::Ghash(int w, int k, int TableSize, int vecSize){
	this->w = w;
	this->k = k;
	this->TableSize = TableSize;
	this->vecSize = vecSize;

	int t; // randomly selected integer 
	std::vector<int> rVec; //randomly selected vector of size vecSize

	for(int i=0; i<k; i++){
		//t = ... rVec = ...
		hVec.push_back({rVec, t, w});
	}
}

Ghash::~Ghash(){}

Ghash::hash(std::vector<int> p){

	int i,r; // r random natural number
	int res=0;
	for(i=0; i<k; i++){
		//r =...
		res += r*hVec[i].hash(p);
	}
	res = res % M; // <----- MUST CORRECT TO CALC. MODULO INSTEAD OF REMAINDER
	res = res % TableSize;
	return res;
}