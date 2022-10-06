#include "HashTable.h"
#include "FileReader.h"
#include "Utilities.h"

#include <vector>
#include <string>

class Point;

class LSH{
    private:
        FileReader &io_files;
        HashTable **hash_tables;
        std::vector<Point*> points;
        int L;
    public:
        LSH(
            FileReader &io_files_ref,
            int w, int k, int l, int hash_table_size=10
        );
        int kNN_Search(int L, int k, PD **b, std::string &id);
        int kNN_Search(int L, int k, PD **b, Point *p);
        int bruteForceNN(std::string &id, int L, int k, PD **b);
        int rangeSearch(int r, std::unordered_map<std::string, Point*> &r_neighbors, std::string &id);
        int rangeSearch(int r, std::unordered_map<std::string, Point*> &r_neighbors, Point *q);
        int reverseRangeSearch(int r, std::unordered_map<std::string, Point*> *Clusters, int k, int k_index, Point **Medoids);
        inline std::vector<Point*>& getAllPoints(){ return this->points; }
        ~LSH();
};


