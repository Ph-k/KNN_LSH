#include "curve.hpp"
#include "point.hpp"
#include "frechet.hpp"
#include "FredLibWrapper.h"
#include "simplification.hpp"

using namespace std;

const static double epsilon = 0.1;

Point *pointVectorToFredPoint(const std::vector<__TIMESERIES_X_TYPE> &p){
    Point *fred_p = new Point(p.size());
    unsigned long i=0;
    for(auto X: p){
        fred_p->set(i++,X);
    }
    return fred_p;
}

vector<__TIMESERIES_X_TYPE> *FredPointToPointVector(Point& simplified_fred_point){
    vector<__TIMESERIES_X_TYPE> *q_vec = new vector<__TIMESERIES_X_TYPE>;
    unsigned long i=0;
    for(auto X: simplified_fred_point){
        q_vec->push_back(X);
    }
    return q_vec;
}

vector<__TIMESERIES_X_TYPE> *Simplify(const std::vector<__TIMESERIES_X_TYPE> &p){
    vector<__TIMESERIES_X_TYPE> *q_vec = new vector<__TIMESERIES_X_TYPE>(p);
    size_t i,size = q_vec->size();
    for(i=1; i<size-1; i++){
        if( abs( q_vec->at(i-1)-q_vec->at(i) ) <= epsilon && abs( q_vec->at(i)-q_vec->at(i+1) ) <= epsilon ){
            q_vec->erase(q_vec->begin() + i);
            i--;
            size = q_vec->size();
        }
    }
    return q_vec;
}

double cfr_distance(TimeSeries *p1, TimeSeries *p2){
    Point *fred_p1 = pointVectorToFredPoint(p1->getXs()), *fred_p2 = pointVectorToFredPoint(p2->getXs());
    Points *fred_dummy_points1 = new Points(p1->getXs().size(),*fred_p1), *fred_dummy_points2 = new Points(p2->getXs().size(),*fred_p2);
    Curve* fred_curve1 = new Curve(*fred_dummy_points1), *fred_curve2 = new Curve(*fred_dummy_points2);

    Frechet::Continuous::Distance fred_dist = Frechet::Continuous::distance(*fred_curve1,*fred_curve2);

    delete fred_curve1; delete fred_curve2;
    delete fred_dummy_points1; delete fred_dummy_points2;
    delete fred_p1; delete fred_p2;
    std::cout << "Distance = " << fred_dist.value << std::endl; 
    return fred_dist.value;
}