#include "ClusterLloyds.h"
#include "Utilities.h"
#include "FileReader.h"

ClusterLloyds::ClusterLloyds(FileReader &io_files_ref,int given_k, char update_metric)
:ClusterInterface(io_files_ref, given_k, &euclidean_distance, update_metric)
{
    // We start by reading the input
    TimeSeries* p = io_files.ReadPoint();
    while( p != nullptr){

        this->points.push_back(p);

        p = io_files.ReadPoint();
    }

    initMedoids();
    Assign();
}

void ClusterLloyds::Assign(){

    int clusterIndex;
    for(long unsigned int i=0; i<points.size(); i++){
        ClusterObject currPoint = points[i];
        clusterIndex = nearestCenter(currPoint);
        Clusters[clusterIndex][currPoint->getId()] = currPoint;
    }
}

ClusterLloyds::~ClusterLloyds()
{
    for(auto point: points)
        delete point;
}