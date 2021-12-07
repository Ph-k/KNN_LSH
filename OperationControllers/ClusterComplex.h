#pragma once

#define __CLASIC_METHOD 0
#define __LSH_METHOD 1
#define __HC_METHOD 2
#define __INVALID_METHOD 3

#include <unordered_map>

#include "Point.h"
#include "SimpleList.h"
#include "Utilities.h"

#define THRESHOLD 0.85

#define ClusterObject Point*

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
        SimpleList *Clusters;
        int *clusterIndexes;
        int clustering_time;
        std::unordered_map<std::string, Point*> *Clusters2;
        void Update(bool first = true);
        void UpdateLSH_HC();
        void Assign();
        void AssignLSH_HC();
        //ClusterObject drawRandomMedoid(const std::vector<ClusterObject>& all_points);
        void initMedoidsPP_LSH_HC();
        char method;
        LSH* LSHController;
        HyperCube* HCController;
        silhouetteStats silhouetteS;
        int findClusterIndex(Point *p);
    public:
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd); //For classic method
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int k_lsh, int l_lsh); //For lsh method
        ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int M_hc, int k_hc, int hc_probes); //For hc method
        ~ClusterComplex();
        void Insert(ClusterObject item);
        int nearestCenter(ClusterObject item, bool sec=false);
        double minDistance(ClusterObject item, int t);
        void kMeans(int epochs);
        inline SimpleList *getClustersList(){return Clusters;};
        inline std::unordered_map<std::string, Point*> *getClustersUmap(){return Clusters2;};
        inline ClusterObject *getMedoids(){return Medoids;};
        inline int getClusteringTimes(){return clustering_time;};
        silhouetteStats *Silhouette();
        silhouetteStats *umapSilhouette();
};

