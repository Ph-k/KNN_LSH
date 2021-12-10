#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>
#include "Hhashing.h"
#include "HashInterface.hpp"
#include "TimeSeries.h"

class HashHC: public HashInterface{
    private:
        std::unordered_map<int, unsigned int> *h_mapping;
        void MaskPermutations(int remBits, std::vector<int> *masks, int threshold, int pos=0, int val=0);
    public:
        HashHC(int w, int k, int TableSize, int vecSize, int probes);
        ~HashHC();
        unsigned int Hash(const std::vector<__TIMESERIES_X_TYPE> &p);
        int *HashIndex(unsigned int ID, bool insertingMode=false);
        std::vector<int> HammingNeighbors(int index, int probes);
};
