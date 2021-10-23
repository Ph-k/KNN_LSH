#include "Utilities.h"

#include <random>

using namespace std;

vector<double>* uniform_vec(int dimensions){
    default_random_engine randomness;
    normal_distribution<double> normal_distribution(0.0,1.0);

    vector<double> *v = new vector<double>;

    for(int i=0; i<dimensions; i++)
        v->push_back(normal_distribution(randomness));

    return v;
}