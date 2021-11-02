#include <vector>
#include "Hhashing.h"

class HashInterface
{
private:
    std::vector<Hhash*> hVec;
    int w;
    int k;
    int TableSize;
    int vecSize;

public:
    HashInterface(int w, int k, int TableSize, int vecSize);
    ~HashInterface();
    virtual unsigned int Hash(const std::vector<int> &p) = 0;
	virtual unsigned int HashIndex(unsigned int ID) = 0;
};

HashInterface::HashInterface(int w, int k, int TableSize, int vecSize)
:w(w), k(k), TableSize(TableSize), vecSize(vecSize)
{
}

HashInterface::~HashInterface()
{
}
