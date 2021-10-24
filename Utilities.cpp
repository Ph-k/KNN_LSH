#include "Utilities.h"

#include <random>

using namespace std;

vector<int>* uniform_vec(int dimensions){
    default_random_engine randomness;
    normal_distribution<float> normal_distribution(0.0,1.0);

    vector<int> *v = new vector<int>;

    for(int i=0; i<dimensions; i++)
        v->push_back((int)normal_distribution(randomness));

    return v;
}

float random_float(int w){
    default_random_engine randomness;
    uniform_real_distribution<float> uniform_distribution(0.0,w);

    return uniform_distribution(randomness);
}

int dot_product(const vector<int> &x,const vector<int> &y){
	if (x.size() != y.size())
		exit(1);
	int n = x.size();
	int dp=0;
	for (int i=0; i<n; i++)
		dp += x[i]*y[i];
	return dp;
}