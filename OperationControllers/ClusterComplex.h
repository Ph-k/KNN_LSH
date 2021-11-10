#pragma once

#define __CLASIC_METHOD 0
#define __LSH_METHOD 1
#define __HC_METHOD 2
#define __INVALID_METHOD 3

#include <unordered_map>

#include "Point.h"
#include "SimpleList.h"
#include "Utilities.h"

#define ClusterObject Point*

class FileReader;
class LSH;

class ClusterComplex{
    private:
        FileReader &io_files;
        int k;
        int k_lsh, l_lsh, lsh_range/*m*/;
        ClusterObject *Medoids;
        std::vector<ClusterObject> points;
        SimpleList *Clusters;
        std::unordered_map<std::string, Point*> *Clusters2;
        void Update();
        void UpdateLSH();
        void Assign();
        void AssignLSH();
        unsigned int* random_medoid_indexes,random_medoid_size;
        ClusterObject drawRandomMedoid(const std::vector<ClusterObject>& all_points);
        char method;
        LSH* LSHController;
    public:
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd); //For classic method
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int k_lsh, int l_lsh); //For lsh method
        ~ClusterComplex();
        void Insert(ClusterObject item);
        int nearestCenter(ClusterObject item);
        void kMeans(int epochs);
        inline SimpleList *getClusters(){return Clusters;};
        inline std::unordered_map<std::string, Point*> *getClusters(int i){return Clusters2;};
        inline ClusterObject *getMedoids(){return Medoids;};
};

