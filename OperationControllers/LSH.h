#include "HashTable.h"
#include "FileReader.h"
#include "Utilities.h"
#include "MappingMethod.hpp"
#include "HashDF.h"

#include <vector>
#include <string>

class TimeSeries;

class LSH: public MappingMethod{
    private:
        HashTable **hash_tables;
        int L;
        HashDF *Frechet;
        char metric;
    public:
        LSH(
            FileReader &io_files_ref,
            int w, int k, int l, int delta, int hash_table_size=10, char metric = __LSH_MODE
        );
        int kNN_Search(int L, int k, PD **b, std::string &id);
        int kNN_Search(int L, int k, PD **b, TimeSeries *q);
        int bruteForceNN(std::string &id, int L, int k, PD **b);
        int bruteForceNN(TimeSeries *q, int L, int k, PD **b);
        int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, std::string &id);
        int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, TimeSeries *q);
        int reverseRangeSearch(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids);
        ~LSH();
};


