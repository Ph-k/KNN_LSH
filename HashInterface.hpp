#pragma once

#include <vector>
#include "Hhashing.h"

class HashInterface
{
    protected:
        std::vector<Hhash*> hVec;
        int w;
        int k;
        int TableSize;
        int vecSize;

    public:
        HashInterface(int w, int k, int TableSize, int vecSize);
        virtual ~HashInterface();
        virtual unsigned int Hash(const std::vector<int> &p) = 0;
        virtual unsigned int HashIndex(unsigned int ID) = 0;
};
