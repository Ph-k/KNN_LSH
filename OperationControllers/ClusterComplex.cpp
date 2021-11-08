#include <float.h>
#include "FileReader.h"
#include "ClusterComplex.h"

ClusterComplex::ClusterComplex(FileReader &io_files_ref,int given_k)
:io_files(io_files_ref), k(given_k){

    // We start by reading the input
    Point* p = io_files.ReadPoint();
    while( p != nullptr){

        this->points.push_back(p);

        p = io_files.ReadPoint();
    }

    Medoids = new ClusterObject[k];

    random_medoid_indexes = new unsigned int[k];
    random_medoid_size = 0;

    Clusters = new SimpleList[k];

    for (int i=0; i<k; i++){
        Medoids[i] = drawRandomMedoid();
        PointPointer pp = {Medoids[i], 0};
        Clusters[i].Push(pp);
    }

    Assign();
    // for (int i=0; i<nonMedoids.size(); i++){
    //     ClusterObject &currPoint = nonMedoids[i];
    //     clusterIndex = nearestCenter(currPoint);
    //     PointPointer pp = {currPoint, euclidean_distance(currPoint, Medoids[clusterIndex])};
    //     Clusters[clusterIndex].Push(pp);
    // }
}

ClusterObject ClusterComplex::drawRandomMedoid(){
    unsigned int randIndex,i;
    bool foundIndex = false;

    randIndex = randUInt(0,this->points.size()-1);
    do{
        foundIndex = true;

        for(i=0; i<random_medoid_size; i++){
            if( random_medoid_indexes[i] == randIndex){
                foundIndex = false;
                randIndex = (randIndex < this->points.size())? randIndex+1 : 0;
                break;
            }
        }

    }while(foundIndex == false);

    random_medoid_size++;

    return this->points.at(randIndex);

}

ClusterComplex::~ClusterComplex()
{
    delete[] Clusters;
    delete[] Medoids;
    delete[] random_medoid_indexes;
}

int ClusterComplex::nearestCenter(ClusterObject item){

    double minDist = DBL_MAX, currDist;
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

    int i;
    PointPointer pp = {nullptr, 0};

    for(i=0; i<k; i++){
        Medoids[i] = Clusters[i].meanVector();
        Clusters[i].empty();

        pp.point = Medoids[i];
        Clusters[i].Push(pp);
    }

}

void ClusterComplex::Assign(){

    int clusterIndex;
    for(long unsigned int i=0; i<points.size(); i++){
        ClusterObject currPoint = points[i];
        clusterIndex = nearestCenter(currPoint);
        PointPointer pp = {currPoint, 0};
        Clusters[clusterIndex].Push(pp);
    }
}

void ClusterComplex::kMeans(int epochs){

    for (int i=0; i<epochs; i++){
        Update();
        Assign();
    }
}