#include <iostream>
#include "TimeSeries.h"

using namespace std;

TimeSeries::TimeSeries(std::vector<__TIMESERIES_X_TYPE> *given_Xs, std::string *given_item_id)
: X(given_Xs), item_id(given_item_id){}

TimeSeries::TimeSeries(TimeSeries &p){
    this->X = new vector<__TIMESERIES_X_TYPE>(p.getXs());
    this->item_id = new string(p.getId());
}

void TimeSeries::print(){
    cout << '{';
    for(auto it: *X)
        cout << it << ", ";
    cout << '}' << endl;
}


TimeSeries::~TimeSeries(){
    delete X;
    delete item_id;
}