#include <vector>
#include <iostream>
#include "Hhash.h"

class Ghash{
	public:
		Ghash(int w, int k, int TableSize, int vecSize);
		~Ghash();
		int hash();
	private:
		std::vector<Hhash> hVec;
		int w;
		int k;
		int TableSize;
		int vecSize;
};