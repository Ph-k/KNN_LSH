#include "ClusterComplex.h"

ClusterComplex::ClusterComplex(int k, std::vector<ClusterObject> &medoids, std::vector<ClusterObject> &nonMedoids)
{
    this->k = k;
    int clusterIndex;
    if(medoids.size() != k){
        std::perror("Invalid size of medoid vector");
    }

    Medoids = new ClusterObject[k];
    Clusters = new SimpleList[k];

    for (int i=0; i<k; i++){
        Medoids[i] = medoids[i];
        PointPointer pp = {Medoids[i], 0};
        Clusters[i].Push(pp);
    }

    Assign(nonMedoids);
    // for (int i=0; i<nonMedoids.size(); i++){
    //     ClusterObject &currPoint = nonMedoids[i];
    //     clusterIndex = nearestCenter(currPoint);
    //     PointPointer pp = {currPoint, euclidean_distance(currPoint, Medoids[clusterIndex])};
    //     Clusters[clusterIndex].Push(pp);
    // }
}

ClusterComplex::~ClusterComplex()
{
    delete[] Clusters;
    delete[] Medoids;
}

int ClusterComplex::nearestCenter(ClusterObject item){

    int minDist = 999999, currDist;
    int index;
    for (int i=0; i<k; i++){
        currDist = euclidean_distance(item, Medoids[i]);
        if (currDist < minDist){
            index = i;
            minDist = currDist;
        }

    }
    return index;
}

void ClusterComplex::Update(){

    for(int i=0; i<k; i++){
        Medoids[i] = Clusters[i].meanVector();
    }
     delete[] Clusters;
}

void ClusterComplex::Assign(std::vector<ClusterObject> &points){

    int clusterIndex;
    for(int i=0; i<points.size(); i++){
        ClusterObject &currPoint = points[i];
        clusterIndex = nearestCenter(currPoint);
        PointPointer pp = {currPoint, euclidean_distance(currPoint, Medoids[clusterIndex])};
        Clusters[clusterIndex].Push(pp);
    }
}

void ClusterComplex::kMeans(std::vector<ClusterObject> &points, int epochs){

    for (int i=0; i<epochs; i++){
        Update();
        Assign(points);
    }
}