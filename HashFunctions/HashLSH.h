#pragma once 

#include <vector>
#include <iostream>
#include "TimeSeries.h"
#include "Hhashing.h"
#include "HashInterface.hpp"
#include "Utilities.h"

class HashDF;

class HashLSH : public HashInterface{
	private:
		char method;
		HashDF* discreteFrechet;
	public:
		HashLSH(int w, int k, int TableSize, int vecSizem, char method = __LSH_MODE, double delta = -1.0);
		~HashLSH();
		unsigned int Hash(const std::vector<__TIMESERIES_X_TYPE> &p);
		int *HashIndex(unsigned int ID, bool insertingMode);
		// inline int getSize(){return TableSize;};
};
