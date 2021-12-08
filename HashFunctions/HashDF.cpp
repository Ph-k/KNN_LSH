#include <iostream>
#include <float.h>
#include <cmath>
#include "HashDF.h"
#include "Utilities.h"

HashDF::HashDF(double delta, int d):delta(delta),d(d),Grid(new double[d]){

    for (int i = 0; i < d; i++)
        Grid[i] = i*delta + random_float(0.0, delta);
    
}

HashDF::~HashDF(){
    delete[] Grid;
}

std::vector<double> *HashDF::Snap(const std::vector<double> &p){
    if (p.size() != d){
        std::stderr << "Size error in Snap function!\n";
    }

    std::vector<double> *q = new std::vector<double>;
    int prev_arg = -1, arg_min = -1;
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