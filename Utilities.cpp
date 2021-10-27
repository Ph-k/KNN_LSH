#include "Utilities.h"
#include "Point.h"

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

float euclidean_distance(Point *p1, Point *p2){
    const vector<int>& Xs_p1 = p1->getXs();
    const vector<int>& Xs_p2 = p2->getXs();

    if(Xs_p1.size() != Xs_p2.size()) exit(1);//Consider removing it for speed

    float sum = 0;

    auto p1_val = Xs_p1.begin();
    auto p2_val = Xs_p2.begin();

    while( p1_val != Xs_p1.end() || p2_val != Xs_p2.end() ){
        sum += (*p2_val - *p1_val)*(*p2_val - *p1_val);
	++p1_val;
	++p2_val;
    }

    return sqrt(sum);
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
