#include <vector>
#include <math.h>
#include <iostream>
#include "CurveArray.h"
#include "TimeSeries.h"

CurveArray::CurveArray(std::unordered_map<std::string, TimeSeries*> &gCluster)
:array( new TimeSeries*[2*gCluster.size()-1] ), arrSize(2*gCluster.size()-1), Cluster(gCluster){
    size_t i=0;
    for(i=0; i<arrSize; i++)
        array[i] = nullptr;
    i = arrSize-gCluster.size();
    for(auto item: gCluster){
        array[i++] = item.second;
    }
    std::cout << "Constructed curve array.\n";
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
    
    recursivePostOrderTraversal(0);
    return array[0];
}

void CurveArray::recursivePostOrderTraversal(unsigned int node_index){

    if(node_index >= arrSize)return;
    unsigned int left_node_index = (node_index+1)*2;
    unsigned int right_node_index = (node_index+1)*2 + 1;
    recursivePostOrderTraversal(left_node_index);
    recursivePostOrderTraversal(right_node_index);
    std::vector<__TIMESERIES_X_TYPE> *mean_vec = nullptr;
    
    if(left_node_index < arrSize && right_node_index < arrSize){
        mean_vec = createMeanVec((array[left_node_index])->getXs(),(array[right_node_index])->getXs());
    }else if(left_node_index < arrSize && right_node_index >= arrSize){
        mean_vec = new std::vector<__TIMESERIES_X_TYPE>((array[left_node_index])->getXs());
    }else if(left_node_index >= arrSize && right_node_index < arrSize){
        std::cout << "Node = " << node_index << std::endl;
        std::cerr << "Illegal move to right node!\n";
    }else if(left_node_index >= arrSize && right_node_index >=arrSize){
        return;
    }
    array[node_index] = new TimeSeries(mean_vec, nullptr);
}

CurveArray::~CurveArray(){
    std::cout << "Destructor called\n";
    delete[] array;
}
