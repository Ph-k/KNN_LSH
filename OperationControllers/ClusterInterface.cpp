#include "ClusterInterface.h"
#include <chrono>
#include <float.h>
#include <string.h>
#include <algorithm>
#include "LSH.h"
#include "FileReader.h"
#include "HyperCube.h"
#include "CurveArray.h"

using namespace std;

TimeSeries *meanVector(std::unordered_map<std::string, TimeSeries*> &Cluster); //F.D.

TimeSeries *meanCurve(std::unordered_map<std::string, TimeSeries*> &Cluster); //F.D.


ClusterInterface::ClusterInterface(FileReader &io_files_ref,int given_k, double (*metric_func)(ClusterObject, ClusterObject), char update_metric)
:io_files(io_files_ref), k(given_k), metric_func(metric_func),Clusters(nullptr){
    silhouetteS.avgSil = nullptr;

    Clusters = new std::unordered_map<std::string, TimeSeries*>[k];

    Medoids = new ClusterObject[k];
    for (int i = 0; i < k; i++)
        Medoids[i] = nullptr;

    switch (update_metric){
        case __MEAN_VEC_UPDATE:
            this->mean_func = &meanVector;
            break;
        case __MEAN_FR_UPDATE:
            this->mean_func = &meanCurve;
            break;
    }

}

void ClusterInterface::initMedoids(){
    vector<TimeSeries*>* allPoints = this->getAllPoints();

    // Initialization ++

    int t=1, n=allPoints->size();
    int randIndex = randUInt(0, n-1);
    std::vector<double> P(n-1);
    std::vector<double> D(n-1);
    Medoids[0] = allPoints->at(randIndex);
    allPoints->erase(allPoints->begin()+randIndex);

    // Generate k Medoids probabillistically.
    for (int i=1; i<k; i++){
        // Use max of all min distances D(i) to normalize
        D[0] = minDistance(allPoints->at(0), t);
        double maxD = D[0];
        for(int j=1; j<n-t; j++){
            D[j] = minDistance(allPoints->at(j), t);
            if(D[j] > maxD) maxD = D[j];
        }
        double currPr = 0.0;
        for(int j=0; j<n-t; j++){
            D[j] = D[j]/maxD;
            P[j] = currPr + D[j]*D[j];
            currPr = P[j];
        }

        // Select double precision number x from uniform distr.
        // Find index r in P so that x <= P(r) and it's at the upper bound
        double randP = (double)random_float(0.0, P[n-t-1]);
        int index = upper_bound(P.begin(), P.end(), randP) - P.begin();
        Medoids[i] = allPoints->at(index);
        allPoints->erase(allPoints->begin()+index);
        Clusters[i][Medoids[i]->getId()] = Medoids[i];
        P.pop_back();
        D.pop_back();
        t++;
    }

    for (int i=0; i<k; i++){
        allPoints->push_back(Medoids[i]);
        Medoids[i] = new TimeSeries(*(Medoids[i]));
    }
}

int ClusterInterface::nearestCenter(ClusterObject item, bool sec){

    double minDist[2] = {DBL_MAX, DBL_MAX}, currDist;
    int index[2]={0,0};
    for (int i=0; i<k; i++){
        currDist = (*metric_func)(item, Medoids[i]);
        if (currDist < minDist[0]){
            minDist[1] = minDist[0];
            minDist[0] = currDist;
            index[1] = index[0];
            index[0] = i;
        }else if(currDist < minDist[1]){
            minDist[1] = currDist;
            index[1] = i;
        }
    }

    if (!sec)
        return index[0];
    return index[1];    
}

double ClusterInterface::minDistance(ClusterObject item, int t){

    double minDist = DBL_MAX, currDist;
    for (int i=0; i<t; i++){
        currDist = (*metric_func)(item, Medoids[i]);
        if (currDist < minDist){
            minDist = currDist;
        }
    }

    return currDist;    
}

TimeSeries *meanVector(std::unordered_map<std::string, TimeSeries*> &Cluster){
    if(Cluster.empty()) return nullptr;
    unsigned int T = Cluster.size();
    int tempVecSize = Cluster.begin()->second->getXs().size();
    double *tempVec = new double[tempVecSize];
    memset(tempVec,0.0,tempVecSize*sizeof(double));
    const std::vector<__TIMESERIES_X_TYPE>* vec;

    for(auto point: Cluster){
        vec = &(point.second->getXs());
        for(int i=0; i<tempVecSize; i++)
            tempVec[i] +=  ( (double)vec->at(i) ) / ((double)T);
    }

    std::vector<__TIMESERIES_X_TYPE> *meanVec = new std::vector<__TIMESERIES_X_TYPE>;
    for(int i=0; i<tempVecSize; i++)
        meanVec->push_back(tempVec[i]);
    std::string *no_s = nullptr;
    TimeSeries *meanP = new TimeSeries(meanVec, no_s);

    delete[] tempVec;
    return meanP;
}

TimeSeries *meanCurve(std::unordered_map<std::string, TimeSeries*> &Cluster){
    CurveArray ca(Cluster);
    return ca.postOrderTraversal();
}

double averageDistance(std::unordered_map<std::string, TimeSeries*> &Cluster, TimeSeries *item){
    if(Cluster.empty()) return 0;
    unsigned int T = Cluster.size();

    double tempDist = 0.0;

    for(auto point: Cluster){
        tempDist += euclidean_distance(point.second, item);
    }
    tempDist = tempDist/(double)T;
    return tempDist;
}

void ClusterInterface::Update(){

    int i;
    TimeSeries *newMedoid;

    for(i=0; i<k; i++){
        newMedoid = this->mean_func(Clusters[i]);
        if(newMedoid != nullptr){
            delete Medoids[i];
            Medoids[i] = newMedoid;
        }

        Clusters[i].clear();
    }

}

void ClusterInterface::kMeans(int epochs){
    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    for (int i=0; i<epochs; i++){
        Update();
        this->Assign();
    }

    clustering_time = (chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - startTime )).count();

}

silhouetteStats *ClusterInterface::Silhouette(){

    double a_i, b_i;
    double s_i;
    if(silhouetteS.avgSil == nullptr) silhouetteS.avgSil = new double[k];
    memset(silhouetteS.avgSil ,0.0,k*sizeof(double));
    silhouetteS.OSC = 0.0;

    vector<TimeSeries*>* allPoints = this->getAllPoints();

    for(long unsigned int i=0; i<allPoints->size(); i++){
        int clusterIndex = findClusterIndex(allPoints->at(i));
        a_i = averageDistance(Clusters[clusterIndex], allPoints->at(i));
        b_i = averageDistance(Clusters[nearestCenter(allPoints->at(i), true)],allPoints->at(i));
        if(a_i == b_i)
            s_i = 0;
        else if(a_i < b_i)
            s_i = 1-a_i/b_i;
        else   
            s_i = b_i/a_i-1;
        silhouetteS.avgSil[clusterIndex] += s_i;
        silhouetteS.OSC += s_i;
    }

    for(int i=0; i<k; i++){
        silhouetteS.avgSil[i] = silhouetteS.avgSil[i]/(double)Clusters[i].size();
    }
    silhouetteS.OSC = silhouetteS.OSC/(double)allPoints->size();

    return &silhouetteS;
}

int ClusterInterface::findClusterIndex(ClusterObject p){
  for(int i=0; i<k; i++){
    if( Clusters[i].find(p->getId()) != Clusters[i].end() ){
      return i;
    }
  }
  return -1;
}

ClusterInterface::~ClusterInterface()
{
    if(Clusters != nullptr) delete[] Clusters;

    for (int i = 0; i < k; i++)
        if(Medoids[i] != nullptr) delete Medoids[i];
    delete[] Medoids;

    // if(LSHController != nullptr) delete LSHController;
    // if(HCController != nullptr) delete HCController;

    if(silhouetteS.avgSil != nullptr) delete silhouetteS.avgSil;
}