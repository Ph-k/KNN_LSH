#include <unordered_map>
#include "FileReader.h"
#include "HashTable.h"

class HyperCube{
    private:
        FileReader &io_files;
        HashTable hash_table;
        std::vector<Point*> points;
        int M;
    public:
        HyperCube(
            FileReader &io_files_ref,
            int w, int k, int M, int probes, int hash_table_size=10);
        int kNN_Search(std::string &id, int L, int k, PD **b);
        int bruteForceNN(std::string &id, int L, int k, PD **b);
        int rangeSearch(std::string &id, int r, std::unordered_map<std::string, Point*> &r_neighbors);
        int reverseRangeSearch(int r, std::unordered_map<std::string, Point*> *Clusters, int k, int k_index, Point **Medoids, int M);
        inline std::vector<Point*>& getAllPoints(){ return this->points; }
        ~HyperCube();
};