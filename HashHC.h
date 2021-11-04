#include <vector>
#include <iostream>
#include "Hhashing.h"
#include "HashInterface.hpp"

class HashHC: public HashInterface{
    public:
        HashHC(int w, int k, int TableSize, int vecSize);
        ~HashHC();
        unsigned int Hash(const std::vector<int> &p);
        inline unsigned int HashIndex(unsigned int ID){return ID;};
};


