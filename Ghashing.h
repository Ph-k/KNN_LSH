#include <vector>
#include <iostream>
#include "Hhashing.h"

class Ghash{
	public:
		Ghash(int w, int k, int TableSize, int vecSize);
		~Ghash();
		unsigned int  hash(const std::vector<int> &p);
		inline int getSize(){return TableSize;};
	private:
		std::vector<Hhash*> hVec;
		int w;
		int k;
		int TableSize;
		int vecSize;
};