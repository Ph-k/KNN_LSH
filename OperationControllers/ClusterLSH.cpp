#include "ClusterLSH.h"

using namespace std;

ClusterLSH::ClusterLSH(FileReader &io_files_ref,int given_k, double (*metric_func)(ClusterObject, ClusterObject),int k_lsh, int l_lsh, char update_metric)
:ClusterInterface(io_files_ref, given_k, metric_func, update_metric), k_lsh(k_lsh), l_lsh(l_lsh), search_range(100)
{
    this->LSHController = new LSH(io_files_ref,150,k_lsh,l_lsh,-1.0,1000);

    initMedoids();
    Assign();
}

inline double ReverseAssignThreshold(unordered_map<string, TimeSeries*> *Clusters2, int k, long unsigned int allPointsCount){
    long unsigned int assignedPoints = 0;
    for(int i=0; i<k; i++){
        assignedPoints += Clusters2[i].size();
    }

    return ((double)assignedPoints)/((double)allPointsCount);
}

void ClusterLSH::Assign(){
    int i;
    int clusterIndex, local_search_range = search_range;
    bool found;
    double R_A_T_Value, R_A_T_Value_old=-1.0;

    const vector<TimeSeries*> *allPoints;

    allPoints = &(LSHController->getAllPoints());

    R_A_T_Value = ReverseAssignThreshold(Clusters,k,allPoints->size());
    while(R_A_T_Value < THRESHOLD && R_A_T_Value != R_A_T_Value_old){
        for(i=0; i<k; i++){
            LSHController->reverseRangeSearch(local_search_range,Clusters,k,i,Medoids);
        }
        local_search_range *= 2;
        R_A_T_Value_old = R_A_T_Value;
        R_A_T_Value = ReverseAssignThreshold(Clusters,k,allPoints->size());
    }

    for(auto point: *allPoints){
        found = false;
        for(i=0; i<k; i++){
            if( Clusters[i].find(point->getId()) != Clusters[i].end() ){
                found = true;
                break;
            }
        }
        if(found == false){
            clusterIndex = nearestCenter(point);
            Clusters[clusterIndex][point->getId()]=point;
        }
    }
}

ClusterLSH::~ClusterLSH(){
    if(LSHController != nullptr) delete LSHController;
}