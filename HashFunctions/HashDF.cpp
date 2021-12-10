#include <iostream>
#include <float.h>
#include <stdio.h>
#include <cmath>
#include "HashDF.h"
#include "Utilities.h"

HashDF::HashDF(double given_delta, int given_d):Grid(new double[given_d]),delta(given_delta),d(given_d){

    for (int i = 0; i < d; i++)
        Grid[i] = i*delta + random_float(0.0, delta);
    
}

HashDF::~HashDF(){
    delete[] Grid;
}

std::vector<__TIMESERIES_X_TYPE> *HashDF::Snap(const std::vector<__TIMESERIES_X_TYPE> &p){
    if (p.size() != (long unsigned int)d){
        perror("Size error in Snap function!\n");
    }

    std::vector<__TIMESERIES_X_TYPE> *q = new std::vector<__TIMESERIES_X_TYPE>;
    int prev_arg = -1;
    for (int i = 0; i < d; i++)
    {
        double dist_i = DBL_MAX;
        int argmin = -1;
        for (int j = 0; i < d; i++)
        {
            double curr_dist = std::abs(p[j] - Grid[j]);
            if (curr_dist < dist_i){
                dist_i = curr_dist;
                argmin = j;
            }
        }
        
        if(prev_arg == -1 || prev_arg != argmin)
            q->push_back(Grid[argmin]);
        prev_arg = argmin;
    }
    return q;
}