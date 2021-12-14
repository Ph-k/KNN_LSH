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
        fred_p[i]=X;
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
    Point *fred_p = pointVectorToFredPoint(p);
    Points *fred_dummy_points = new Points(p.size(),*fred_p);
    Curve* fred_curve = new Curve(*fred_dummy_points);

    Curve simplified_fred_curve = Simplification::approximate_minimum_link_simplification(*fred_curve,epsilon);

    Point& simplified_fred_point = simplified_fred_curve.front();

    vector<__TIMESERIES_X_TYPE> *q_vec = FredPointToPointVector(simplified_fred_point);

    delete fred_curve;
    delete fred_dummy_points;
    delete fred_p;

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

    return fred_dist.value;
}