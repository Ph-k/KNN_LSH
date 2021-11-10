#include <float.h>
#include <string.h>
#include "LSH.h"
#include "FileReader.h"
#include "ClusterComplex.h"
#include "HyperCube.h"

using namespace std;

ClusterComplex::ClusterComplex(FileReader &io_files_ref,int given_k, char mthd)
:io_files(io_files_ref), k(given_k), Clusters2(nullptr), LSHController(nullptr), HCController(nullptr){

    if(mthd != __CLASIC_METHOD){
        cerr << "Cluster(::ClusterComplex): non Classic method requested to classic constructor" << endl;
        exit(-3);
    }else this->method = mthd;

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
        Medoids[i] = drawRandomMedoid(this->points);
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

ClusterComplex::ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int k_lsh, int l_lsh)
:io_files(io_files_ref), k(given_k), k_lsh(k_lsh), l_lsh(l_lsh), search_range(100), Clusters(nullptr), HCController(nullptr){

    if(mthd != __LSH_METHOD){
        cerr << "Cluster(::ClusterComplex): non LSH method requested to LSH constructor" << endl;
        exit(-3);
    }else this->method = mthd;

    this->LSHController = new LSH(io_files_ref,150,k_lsh,l_lsh,1000);

    Clusters2 = new std::unordered_map<std::string, Point*>[k];


    Medoids = new ClusterObject[k];

    random_medoid_indexes = new unsigned int[k];
    random_medoid_size = 0;

    for (int i=0; i<k; i++){
        Medoids[i] = drawRandomMedoid(LSHController->getAllPoints());
    }

    AssignLSH_HC();
}

ClusterComplex::ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int M_hc, int k_hc, int hc_probes)
:io_files(io_files_ref), k(given_k), search_range(100), M_hc(M_hc), k_hc(k_hc), hc_probes(hc_probes), Clusters(nullptr), LSHController(nullptr){
    if(mthd != __HC_METHOD){
        cerr << "Cluster(::ClusterComplex): non hyper cube method requested to hyper cube constructor" << endl;
        exit(-3);
    }else this->method = mthd;

    this->HCController = new HyperCube(io_files_ref,150,k_hc,hc_probes,1000);

    Clusters2 = new std::unordered_map<std::string, Point*>[k];


    Medoids = new ClusterObject[k];

    random_medoid_indexes = new unsigned int[k];
    random_medoid_size = 0;

    for (int i=0; i<k; i++){
        Medoids[i] = drawRandomMedoid(HCController->getAllPoints());
    }

    AssignLSH_HC();
}

ClusterObject ClusterComplex::drawRandomMedoid(const vector<ClusterObject>& all_points){
    unsigned int randIndex,i;
    bool foundIndex = false;

    randIndex = randUInt(0,all_points.size()-1);
    do{
        foundIndex = true;

        for(i=0; i<random_medoid_size; i++){
            if( random_medoid_indexes[i] == randIndex){
                foundIndex = false;
                randIndex = (randIndex < all_points.size())? randIndex+1 : 0;
                break;
            }
        }

    }while(foundIndex == false);

    random_medoid_size++;

    return all_points.at(randIndex);

}

ClusterComplex::~ClusterComplex()
{
    if(Clusters != nullptr) delete[] Clusters;
    delete[] Medoids;
    delete[] random_medoid_indexes;

    if(LSHController != nullptr) delete LSHController;
    if(Clusters2 != nullptr) delete[] Clusters2;
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

Point *meanVector(std::unordered_map<std::string, Point*> &Cluster){
    if(Cluster.empty()) return nullptr;
    unsigned int T = Cluster.size();
    int tempVecSize = Cluster.begin()->second->getXs().size();
    double *tempVec = new double[tempVecSize];
    memset(tempVec,0.0,tempVecSize*sizeof(double));
    const std::vector<int>* vec;

    for(auto point: Cluster){
        vec = &(point.second->getXs());
        for(int i=0; i<tempVecSize; i++)
            tempVec[i] +=  ( (double)vec->at(i) ) / ((double)T);
    }

    std::vector<int> *meanVec = new std::vector<int>;
    for(int i=0; i<tempVecSize; i++)
        meanVec->push_back((int)tempVec[i]);
    std::string *no_s = nullptr;
    Point *meanP = new Point(meanVec, no_s);

    delete[] tempVec;
    return meanP;
}

void ClusterComplex::UpdateLSH_HC(){

    int i;
    Point *newMedoid;

    for(i=0; i<k; i++){
        newMedoid = meanVector(Clusters2[i]);
        Medoids[i] = (newMedoid != nullptr? newMedoid : Medoids[i]);

        Clusters2[i].clear();
    }

}

void ClusterComplex::AssignLSH_HC(){
    int i, clusterIndex;
    bool found;

    const vector<Point*> *allPoints;

    switch (this->method){
        case __LSH_METHOD:
            for(i=0; i<k; i++){
                LSHController->reverseRangeSearch(search_range,Clusters2,k,i,Medoids);
            }
            allPoints = &(LSHController->getAllPoints());
            break;
        default:
            for(i=0; i<k; i++){
                HCController->reverseRangeSearch(search_range,Clusters2,k,i,Medoids);
            }
            allPoints = &(HCController->getAllPoints());
            break;
    }

    search_range *= 2;

    points.clear();
    for(auto point: *allPoints){
        found = false;
        for(i=0; i<k; i++){
            if( Clusters2[i].find(point->getId()) != Clusters2[i].end() ){
                found = true;
                break;
            }
        }
        if(found == false){
            clusterIndex = nearestCenter(point);
            Clusters2[clusterIndex][point->getId()]=point;
        }
    }
}



void ClusterComplex::kMeans(int epochs){

    for (int i=0; i<epochs; i++){
        switch (this->method){
            case __CLASIC_METHOD:
                Update();
                Assign();
                break;
            default:
                UpdateLSH_HC();
                AssignLSH_HC();
                break;
        }
    }

}