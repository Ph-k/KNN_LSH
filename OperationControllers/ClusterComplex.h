#pragma once

#include "Point.h"
#include "SimpleList.h"
#include "Utilities.h"

#define ClusterObject Point*

class FileReader;

class ClusterComplex{
    private:
        FileReader &io_files;
        int k;
        ClusterObject *Medoids;
        std::vector<ClusterObject> points;
        SimpleList *Clusters;
        void Update();
        void Assign();
        unsigned int* random_medoid_indexes,random_medoid_size;
        ClusterObject drawRandomMedoid();
    public:
        ClusterComplex(FileReader &io_files_ref,int given_k);
        ~ClusterComplex();
        void Insert(ClusterObject item);
        int nearestCenter(ClusterObject item);
        void kMeans(int epochs);
        inline SimpleList *getClusters(){return Clusters;};
        inline ClusterObject *getMedoids(){return Medoids;};
};

