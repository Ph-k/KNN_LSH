#include <chrono>
#include <float.h>
#include <string.h>
#include <algorithm>
#include "LSH.h"
#include "FileReader.h"
#include "ClusterComplex.h"
#include "HyperCube.h"

using namespace std;

ClusterComplex::ClusterComplex(FileReader &io_files_ref,int given_k, char mthd)
:io_files(io_files_ref), k(given_k), Clusters2(nullptr), LSHController(nullptr), HCController(nullptr){
    silhouetteS.avgSil = nullptr;

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
    clusterIndexes = new int[points.size()];

    /*random_medoid_indexes = new unsigned int[k];
    random_medoid_size = 0;*/

    Clusters = new SimpleList[k];

    // for (int i=0; i<k; i++){
    //     Medoids[i] = drawRandomMedoid(this->points);
    //     PointPointer pp = {Medoids[i], 0};
    //     Clusters[i].Push(pp);
    // }

    // Initialization ++

    int t=1, n=points.size();
    int randIndex = randUInt(0, n-1);
    std::vector<double> P(n-1);
    std::vector<double> D(n-1);
    Medoids[0] = points[randIndex];
    points.erase(points.begin()+randIndex);

    // Generate k Medoids probabillistically.
    for (int i=1; i<k; i++){
        // Use max of all min distances D(i) to normalize
        D[0] = minDistance(points[0], t);
        double maxD = D[0];
        for(int j=1; j<n-t; j++){
            D[j] = minDistance(points[j], t);
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
        Medoids[i] = points[index];
        points.erase(points.begin()+index);
        PointPointer pp = {Medoids[i], 0};
        Clusters[i].Push(pp);
        P.pop_back();
        D.pop_back();
        t++;
    }

    for (int i=0; i<k; i++){
        points.push_back(Medoids[i]);
    }
    Assign();
}


void ClusterComplex::initMedoidsPP_LSH_HC(){
    vector<Point*>* allPoints;
    switch (this->method){
        case __LSH_METHOD:
            allPoints = &(LSHController->getAllPoints());
            break;
        default:
            allPoints = &(HCController->getAllPoints());
            break;
    }

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
        Clusters2[i][Medoids[i]->getId()] = Medoids[i];
        P.pop_back();
        D.pop_back();
        t++;
    }

    for (int i=0; i<k; i++){
        allPoints->push_back(Medoids[i]);
        Medoids[i] = new Point(*(Medoids[i]));
    }
}

ClusterComplex::ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int k_lsh, int l_lsh)
:io_files(io_files_ref), k(given_k), k_lsh(k_lsh), l_lsh(l_lsh), search_range(100), Clusters(nullptr), clusterIndexes(nullptr), HCController(nullptr){
    silhouetteS.avgSil = nullptr;

    if(mthd != __LSH_METHOD){
        cerr << "Cluster(::ClusterComplex): non LSH method requested to LSH constructor" << endl;
        exit(-3);
    }else this->method = mthd;

    this->LSHController = new LSH(io_files_ref,150,k_lsh,l_lsh,1000);

    Clusters2 = new std::unordered_map<std::string, Point*>[k];

    Medoids = new ClusterObject[k];
    for (int i = 0; i < k; i++)
        Medoids[i] = nullptr;

    /*random_medoid_indexes = new unsigned int[k];
    random_medoid_size = 0;*/

    initMedoidsPP_LSH_HC();

    AssignLSH_HC();
}

ClusterComplex::ClusterComplex(FileReader &io_files_ref,int given_k, char mthd, int M_hc, int k_hc, int hc_probes)
:io_files(io_files_ref), k(given_k), search_range(100), M_hc(M_hc), k_hc(k_hc), hc_probes(hc_probes), Clusters(nullptr), clusterIndexes(nullptr), LSHController(nullptr){
    silhouetteS.avgSil = nullptr;

    if(mthd != __HC_METHOD){
        cerr << "Cluster(::ClusterComplex): non hyper cube method requested to hyper cube constructor" << endl;
        exit(-3);
    }else this->method = mthd;

    this->HCController = new HyperCube(io_files_ref,150,k_hc,hc_probes,1000);

    Clusters2 = new std::unordered_map<std::string, Point*>[k];


    Medoids = new ClusterObject[k];
    for (int i = 0; i < k; i++)
        Medoids[i] = nullptr;

    initMedoidsPP_LSH_HC();

    AssignLSH_HC();
}


// drawRandomMedoid was used for the simple kmeans Lloyd's implimentation (not kmeans++)
/*ClusterObject ClusterComplex::drawRandomMedoid(const vector<ClusterObject>& all_points){
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

}*/

ClusterComplex::~ClusterComplex()
{
    if(Clusters != nullptr) delete[] Clusters;

    for (int i = 0; i < k; i++)
        if(Medoids[i] != nullptr) delete Medoids[i];
    delete[] Medoids;

    //delete[] random_medoid_indexes;

    if(LSHController != nullptr) delete LSHController;
    if(HCController != nullptr) delete HCController;
    if(Clusters2 != nullptr) delete[] Clusters2;

    if(clusterIndexes != nullptr) delete[] clusterIndexes;

    if(silhouetteS.avgSil != nullptr) delete silhouetteS.avgSil;

    for(auto point: points)
        delete point;

}

int ClusterComplex::nearestCenter(ClusterObject item, bool sec){

    double minDist[2] = {DBL_MAX, DBL_MAX}, currDist;
    int index[2]={0,0};
    for (int i=0; i<k; i++){
        currDist = euclidean_distance(item, Medoids[i]);
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

double ClusterComplex::minDistance(ClusterObject item, int t){

    double minDist = DBL_MAX, currDist;
    for (int i=0; i<t; i++){
        currDist = euclidean_distance(item, Medoids[i]);
        if (currDist < minDist){
            minDist = currDist;
        }
    }

    return currDist;    
}

void ClusterComplex::Update(bool first){

    int i;
    PointPointer pp = {nullptr, 0};

    for(i=0; i<k; i++){
        
        ClusterObject temp = Medoids[i];
        Medoids[i] = Clusters[i].meanVector();
        Clusters[i].empty();
        if(!first)
            delete temp;

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
        clusterIndexes[i] = clusterIndex;
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

double averageDistance(std::unordered_map<std::string, Point*> &Cluster, Point *item){
    if(Cluster.empty()) return 0;
    unsigned int T = Cluster.size();

    double tempDist = 0.0;

    for(auto point: Cluster){
        tempDist += euclidean_distance(point.second, item);
    }
    tempDist = tempDist/(double)T;
    return tempDist;
}

void ClusterComplex::UpdateLSH_HC(){

    int i;
    Point *newMedoid;

    for(i=0; i<k; i++){
        newMedoid = meanVector(Clusters2[i]);
        if(newMedoid != nullptr){
            delete Medoids[i];
            //if(Medoids[i]->getId() != nullptr) delete Medoids[i];
            Medoids[i] = newMedoid;
        }

        Clusters2[i].clear();
    }

}

inline double ReverseAssignLSH_HCThreshold(unordered_map<string, Point*> *Clusters2, int k, long unsigned int allPointsCount){
    long unsigned int assignedPoints = 0;
    for(int i=0; i<k; i++){
        assignedPoints += Clusters2[i].size();
    }

    return ((double)assignedPoints)/((double)allPointsCount);
}

void ClusterComplex::AssignLSH_HC(){
    int i;
    int clusterIndex, local_search_range = search_range;
    bool found;
    double R_A_T_Value, R_A_T_Value_old=-1.0;

    const vector<Point*> *allPoints;

    switch (this->method){
        case __LSH_METHOD:
            allPoints = &(LSHController->getAllPoints());
            break;
        default:
            allPoints = &(HCController->getAllPoints());
            break;
    }

    R_A_T_Value = ReverseAssignLSH_HCThreshold(Clusters2,k,allPoints->size());
    while(R_A_T_Value < THRESHOLD && R_A_T_Value != R_A_T_Value_old){
        switch (this->method){
            case __LSH_METHOD:
                for(i=0; i<k; i++){
                    LSHController->reverseRangeSearch(local_search_range,Clusters2,k,i,Medoids);
                }
                break;
            default:
                for(i=0; i<k; i++){
                    HCController->reverseRangeSearch(local_search_range,Clusters2,k,i,Medoids,this->M_hc);
                }
                break;
        }
        local_search_range *= 2;
        R_A_T_Value_old = R_A_T_Value;
        R_A_T_Value = ReverseAssignLSH_HCThreshold(Clusters2,k,allPoints->size());
    }


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
    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    bool first = true;
    for (int i=0; i<epochs; i++){
        switch (this->method){
            case __CLASIC_METHOD:
                Update(first);
                Assign();
                break;
            default:
                UpdateLSH_HC();
                AssignLSH_HC();
                break;
        }
        first = false;
    }

    clustering_time = (chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - startTime )).count();

}

silhouetteStats *ClusterComplex::Silhouette(){

    double a_i, b_i;
    double s_i;
    if(silhouetteS.avgSil == nullptr) silhouetteS.avgSil = new double[k];

    for(long unsigned int i=0; i<points.size(); i++){
        a_i = Clusters[clusterIndexes[i]].averageDistance(points[i]);
        b_i = Clusters[nearestCenter(points[i], true)].averageDistance(points[i]);
        if(a_i == b_i)
            s_i = 0;
        else if(a_i < b_i)
            s_i = 1-a_i/b_i;
        else   
            s_i = b_i/a_i-1;
        silhouetteS.avgSil[clusterIndexes[i]] += s_i;
        silhouetteS.OSC += s_i;
    }
   
    for(int i=0; i<k; i++){
        silhouetteS.avgSil[i] = silhouetteS.avgSil[i]/(double)Clusters[i].size();
    }
    silhouetteS.OSC = silhouetteS.OSC/(double)points.size();

    return &silhouetteS;
}

silhouetteStats *ClusterComplex::umapSilhouette(){

    double a_i, b_i;
    double s_i;
    if(silhouetteS.avgSil == nullptr) silhouetteS.avgSil = new double[k];
    memset(silhouetteS.avgSil ,0.0,k*sizeof(double));
    silhouetteS.OSC = 0.0;

    vector<Point*>* allPoints;
    switch (this->method){
        case __LSH_METHOD:
            allPoints = &(LSHController->getAllPoints());
            break;
        default:
            allPoints = &(HCController->getAllPoints());
            break;
    }

    for(long unsigned int i=0; i<allPoints->size(); i++){
        int clusterIndex = findClusterIndex(allPoints->at(i));
        a_i = averageDistance(Clusters2[clusterIndex], allPoints->at(i));
        b_i = averageDistance(Clusters2[nearestCenter(allPoints->at(i), true)],allPoints->at(i));
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
        silhouetteS.avgSil[i] = silhouetteS.avgSil[i]/(double)Clusters2[i].size();
    }
    silhouetteS.OSC = silhouetteS.OSC/(double)allPoints->size();

    return &silhouetteS;
}

int ClusterComplex::findClusterIndex(Point *p){
  for(int i=0; i<k; i++){
    if( Clusters2[i].find(p->getId()) != Clusters2[i].end() ){
      return i;
    }
  }
  return -1;
}