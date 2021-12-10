#pragma once 

#include <vector>
#include <iostream>
#include "TimeSeries.h"
#include "Hhashing.h"
#include "HashInterface.hpp"

class HashLSH : public HashInterface{
	public:
		HashLSH(int w, int k, int TableSize, int vecSize);
		~HashLSH();
		unsigned int Hash(const std::vector<__TIMESERIES_X_TYPE> &p);
		int *HashIndex(unsigned int ID, bool insertingMode);
		// inline int getSize(){return TableSize;};
};
