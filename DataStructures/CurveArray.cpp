#include <vector>
#include <math.h>
#include "CurveArray.h"
#include "TimeSeries.h"

CurveArray::CurveArray(std::unordered_map<std::string, TimeSeries*> &gCluster)
:array( new TimeSeries*[gCluster.size()] ),Cluster(gCluster){
    size_t i = 0;
    for(auto item: gCluster){
        array[i++] = item.second;
    }
    //shuffle here
}

// t1.size == t2.size
std::vector<__TIMESERIES_X_TYPE> *createMeanVec(const std::vector<__TIMESERIES_X_TYPE>& t1, const std::vector<__TIMESERIES_X_TYPE> &t2){
    std::vector<__TIMESERIES_X_TYPE> *mean_vec = new std::vector<__TIMESERIES_X_TYPE>;
    for(size_t i=0; i<t1.size(); i++){
        mean_vec->push_back((t1.at(i)+t2.at(i))/2.0);
    }
    return mean_vec;
}

TimeSeries *CurveArray::postOrderTraversal(){
    unsigned int j,max_j = log2(Cluster.size());
    unsigned int i,max_i = Cluster.size();
    std::vector<__TIMESERIES_X_TYPE> *mean_vec = new std::vector<__TIMESERIES_X_TYPE>;
    for(j=0; j<max_j; j++){
        for(i=0; i<max_i; i+=(int)pow(2,j+1)){
            if(i+(int)pow(2,j)>=max_i) break;
            mean_vec = createMeanVec((array[i])->getXs(),(array[i+(int)pow(2,j)])->getXs());
            array[i] = new TimeSeries(mean_vec, nullptr);
        }
    }
    return array[0];
}

CurveArray::~CurveArray(){
    unsigned int j,max_j = log2(Cluster.size());
    unsigned int i,max_i = Cluster.size();
    for(j=0; j<max_j; j++){
        for(i=0; i<max_i; i+=(int)pow(2,j+1)){
            //delete array[i];
        }
    }
    delete[] array;
}
