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

class ClusterInterface
{
protected:
        FileReader &io_files;
        int k;
        double (*metric_func)(ClusterObject, ClusterObject);
        //int k_lsh, l_lsh, search_range, M_hc, k_hc, hc_probes;
        ClusterObject *Medoids;
        int *clusterIndexes;
        int clustering_time;
        std::unordered_map<std::string, TimeSeries*> *Clusters;
        void Update();
        virtual void Assign() = 0;
        // void AssignLSH_HC();
        void initMedoids();
        // LSH* LSHController;
        // HyperCube* HCController;
        silhouetteStats silhouetteS;
        int findClusterIndex(TimeSeries *p);
        virtual std::vector<TimeSeries*>* getAllPoints() = 0;
public:
    ClusterInterface(FileReader &io_files_ref,int given_k, double (*metric_func)(ClusterObject, ClusterObject));
    virtual ~ClusterInterface();
    void Insert(ClusterObject item);
    int nearestCenter(ClusterObject item, bool sec=false);
    double minDistance(ClusterObject item, int t);
    void kMeans(int epochs);
    inline std::unordered_map<std::string, TimeSeries*> *getClusters(){return Clusters;};
    inline ClusterObject *getMedoids(){return Medoids;};
    inline int getClusteringTimes(){return clustering_time;};
    silhouetteStats *Silhouette();
};
