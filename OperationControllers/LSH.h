#include "HashTable.h"
#include "FileReader.h"
#include "Utilities.h"

#include <vector>
#include <string>

class TimeSeries;

class LSH{
    private:
        FileReader &io_files;
        HashTable **hash_tables;
        std::vector<TimeSeries*> points;
        int L;
    public:
        LSH(
            FileReader &io_files_ref,
            int w, int k, int l, int hash_table_size=10
        );
        int kNN_Search(int L, int k, PD **b, std::string &id);
        int kNN_Search(int L, int k, PD **b, TimeSeries *p);
        int bruteForceNN(std::string &id, int L, int k, PD **b);
        int bruteForceNN(TimeSeries *q, int L, int k, PD **b);
        int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, std::string &id);
        int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, TimeSeries *q);
        int reverseRangeSearch(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids);
        inline std::vector<TimeSeries*>& getAllPoints(){ return this->points; }
        ~LSH();
};


