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
        int numProbes;
    public:
        HashInterface(int w, int k, int TableSize, int vecSize, int probes);
        virtual ~HashInterface();
        virtual unsigned int Hash(const std::vector<double> &p) = 0;
        inline int getNumProbes(){return numProbes;};
        virtual int *HashIndex(unsigned int ID, bool insertingMode = false) = 0;
};
