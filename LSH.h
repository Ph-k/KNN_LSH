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
        int kNN_Search(std::string &id, int L, int k, PD **b);
        int bruteForceNN(std::string &id, int L, int k, PD **b);
        ~LSH();
};


