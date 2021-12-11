#pragma once

#define __CLASIC_METHOD 0
#define __LSH_METHOD 1
#define __HC_METHOD 2
#define __INVALID_METHOD 3

#include <unordered_map>

#include "TimeSeries.h"
#include "SimpleList.h"
#include "Utilities.h"

#define THRESHOLD 0.85

#define ClusterObject TimeSeries*

struct silhouetteStats{
    double *avgSil;
    double OSC;
};

class FileReader;
class LSH;
class HyperCube;

class ClusterComplex{
    private:
        FileReader &io_files;
        int k;
        int k_lsh, l_lsh, search_range, M_hc, k_hc, hc_probes;
        ClusterObject *Medoids;
        std::vector<ClusterObject> points;
        int *clusterIndexes;
        int clustering_time;
        std::unordered_map<std::string, TimeSeries*> *Clusters;
        //void Update(bool first = true);
        void Update();
        void AssignLloyds();
        void AssignLSH_HC();
        void initMedoids();
        char method;
        LSH* LSHController;
        HyperCube* HCController;
        silhouetteStats silhouetteS;
        int findClusterIndex(TimeSeries *p);
    public:
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd); //For classic method
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int k_lsh, int l_lsh); //For lsh method
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int M_hc, int k_hc, int hc_probes); //For hc method
        ~ClusterComplex();
        void Insert(ClusterObject item);
        int nearestCenter(ClusterObject item, bool sec=false);
        double minDistance(ClusterObject item, int t);
        void kMeans(int epochs);
        inline std::unordered_map<std::string, TimeSeries*> *getClusters(){return Clusters;};
        inline ClusterObject *getMedoids(){return Medoids;};
        inline int getClusteringTimes(){return clustering_time;};
        //silhouetteStats *Silhouette();
        silhouetteStats *Silhouette();
};

