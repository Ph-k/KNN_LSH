#include <vector>
#include <iostream>
#include "Hhashing.h"
#include "HashInterface.hpp"

class HashHC: public HashInterface
{
private:
    /* data */
public:
    HashHC(int w, int k, int TableSize, int vecSize);
    ~HashHC();
    unsigned int HashHC::Hash(const std::vector<int> &p);
    inline unsigned int HashHC::HashIndex(unsigned int ID){return ID;};
};


