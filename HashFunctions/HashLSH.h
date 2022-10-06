#include <vector>
#include <iostream>
#include "Hhashing.h"
#include "HashInterface.hpp"

class HashLSH : public HashInterface{
	public:
		HashLSH(int w, int k, int TableSize, int vecSize);
		~HashLSH();
		unsigned int Hash(const std::vector<int> &p);
		int *HashIndex(unsigned int ID, bool insertingMode);
		// inline int getSize(){return TableSize;};
};
