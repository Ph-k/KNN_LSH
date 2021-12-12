#include "ClusterHC.h"
#include "Utilities.h"

using namespace std;

ClusterHC::ClusterHC(FileReader &io_files_ref,int given_k, int M_hc, int k_hc, int hc_probes)
:ClusterInterface(io_files_ref, given_k, &euclidean_distance, __MEAN_VEC_UPDATE), M_hc(M_hc), k_hc(k_hc), hc_probes(hc_probes), search_range(100)
{
    this->HCController = new HyperCube(io_files_ref,150,k_hc,hc_probes,1000);

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

void ClusterHC::Assign(){
    int i;
    int clusterIndex, local_search_range = search_range;
    bool found;
    double R_A_T_Value, R_A_T_Value_old=-1.0;
    const vector<TimeSeries*> *allPoints;

    allPoints = &(HCController->getAllPoints());

    R_A_T_Value = ReverseAssignThreshold(Clusters,k,allPoints->size());
    while(R_A_T_Value < THRESHOLD && R_A_T_Value != R_A_T_Value_old){
        for(i=0; i<k; i++){
            HCController->reverseRangeSearch(local_search_range,Clusters,k,i,Medoids,this->M_hc);
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

ClusterHC::~ClusterHC()
{
}