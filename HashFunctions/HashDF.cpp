#include <iostream>
#include <float.h>
#include <stdio.h>
#include <cmath>
#include "HashDF.h"
#include "Utilities.h"

HashDF::HashDF(double given_delta, int given_d):t(new double[given_d]),delta(given_delta),d(given_d){

    for (int i = 0; i < d; i++)
        t[i] = random_float(0.0, delta);
    
}

HashDF::~HashDF(){
    delete[] t;
}

std::vector<__TIMESERIES_X_TYPE> *HashDF::Snap(const std::vector<__TIMESERIES_X_TYPE> &p){
    if (p.size() != (long unsigned int)d){
        std::cerr << "Size error in Snap function!" << std::endl;
    }

    std::vector<__TIMESERIES_X_TYPE> *q = new std::vector<__TIMESERIES_X_TYPE>;
    int prev_arg, argmin;
    for (int i = 0; i < d; i++){
        argmin = floor( (p[i]-t[i])/delta + 0.5 ) * delta + t[i];
        
        if(i == 0 || prev_arg != argmin)
            q->push_back(argmin);

        prev_arg = argmin;
    }
    return q;
}