#include <vector>
#include <iostream>
#include <unordered_map>
#include "Hhashing.h"
#include "HashInterface.hpp"

class HashHC: public HashInterface{
    private:
        std::unordered_map<int, unsigned int> *h_mapping;
        void MaskPermutations(int remBits, std::vector<int> *masks, int &threshold, int pos=0, int val=0);
    public:
        HashHC(int w, int k, int TableSize, int vecSize);
        ~HashHC();
        unsigned int Hash(const std::vector<int> &p);
        inline unsigned int HashIndex(unsigned int ID){return ID;};
        std::vector<int>  HammingNeighbors(int index, int probes, int dim);
};
