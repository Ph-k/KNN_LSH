#include <vector>
#include <iostream>
#include "Hhashing.h"
#include "HashInterface.hpp"

class HashLSH : public HashInterface{
	public:
		HashLSH(int w, int k, int TableSize, int vecSize);
		~HashLSH();
		unsigned int Hash(const std::vector<int> &p);
		inline unsigned int HashIndex(unsigned int ID){return ID % TableSize;};
		// inline int getSize(){return TableSize;};
};
