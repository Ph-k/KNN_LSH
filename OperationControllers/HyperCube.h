#include <unordered_map>
#include "FileReader.h"
#include "HashTable.h"
#include "MappingMethod.hpp"

class HyperCube: public MappingMethod{
    private:
        HashTable hash_table;
        int M;
    public:
        HyperCube(
            FileReader &io_files_ref,
            int w, int k, int M, int probes, int hash_table_size=10);
        int kNN_Search(int L, int k, PD **b, std::string &id);
        int kNN_Search(int L, int k, PD **b, TimeSeries *q);
        int bruteForceNN(std::string &id, int L, int k, PD **b);
        int bruteForceNN(TimeSeries *q, int L, int k, PD **b);
        int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, std::string &id);
        int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, TimeSeries *q);
        int reverseRangeSearch(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids, int M);
        ~HyperCube();
};
