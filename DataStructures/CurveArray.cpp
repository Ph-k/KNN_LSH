#include <vector>
#include <math.h>
#include <iostream>
#include "CurveArray.h"
#include "TimeSeries.h"

CurveArray::CurveArray(std::unordered_map<std::string, TimeSeries*> &gCluster)
:/*array(new TimeSeries*[2*gCluster.size()-1] ),*/ arrSize(2*gCluster.size()-1), Cluster(gCluster){
    if (gCluster.size() == 0){
        array = nullptr;
    }else{
        array = new TimeSeries*[2*gCluster.size()-1];
        std::cout << "size of array: "  << arrSize << std::endl;
        size_t i=0;
        for(i=0; i<arrSize; i++)
            array[i] = nullptr;
        i = arrSize-gCluster.size();
        for(auto item: gCluster){
            array[i++] = item.second;
        }
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
    if(!array){
        return nullptr;
    }else if(Cluster.size() == 1){// Edge case, imposible to traverse tree of size 1
        std::vector<__TIMESERIES_X_TYPE> *mean_vec = new std::vector<__TIMESERIES_X_TYPE>;
        for(auto X: (Cluster.begin()->second)->getXs() )
            mean_vec->push_back(X);
        return new TimeSeries(mean_vec, nullptr);
    }else{
        recursivePostOrderTraversal(0);
    }

    return array[0];
}

void CurveArray::recursivePostOrderTraversal(unsigned int node_index){

    if(node_index >= arrSize) return;
    unsigned int left_node_index = (2*node_index)+1;
    unsigned int right_node_index = (2*node_index)+2;
    recursivePostOrderTraversal(left_node_index);
    recursivePostOrderTraversal(right_node_index);
    std::vector<__TIMESERIES_X_TYPE> *mean_vec = nullptr;

    if(left_node_index < arrSize && right_node_index < arrSize){
        mean_vec = createMeanVec((array[left_node_index])->getXs(),(array[right_node_index])->getXs());
    }else if(left_node_index < arrSize && right_node_index >= arrSize){
        mean_vec = new std::vector<__TIMESERIES_X_TYPE>;
        for(auto X: (array[left_node_index])->getXs() )
            mean_vec->push_back(X);
    }else if(left_node_index >= arrSize && right_node_index < arrSize){
        std::cerr << "Node = " << node_index << std::endl;
    }else if(left_node_index >= arrSize && right_node_index >=arrSize){
        return;
    }
    std::cerr << "Node = " << node_index << std::endl;
    array[node_index] = new TimeSeries(mean_vec, nullptr);
}

CurveArray::~CurveArray(){
    if(array == nullptr)
        return;
    // if(Cluster.size() != 1)
    //     for(size_t i=1; i<arrSize-Cluster.size(); i++)
    //         if( array[i] != nullptr ) delete array[i];
    // delete[] array;
}
