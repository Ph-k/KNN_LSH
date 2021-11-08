#include "FileReader.h"
#include "Utilities.h"
#include "Point.h"

#include <random>
#include <time.h>

using namespace std;

static default_random_engine randomness;
static bool seeded = false;

// Create a vector object with floating numbers taken from the normal distribution
vector<int>* uniform_vec(int dimensions){
    if(seeded == false) { randomness.seed(time(NULL)); seeded=true; }
    normal_distribution<float> normal_distribution(0.0,1.0);

    vector<int> *v = new vector<int>;

    for(int i=0; i<dimensions; i++)
        v->push_back((int)normal_distribution(randomness));

    return v;
}

// Choose random floating point number in the inerval (0,w] from uniform distribution
float random_float(float l, float h){
    if(seeded == false) { randomness.seed(time(NULL)); seeded=true; }
    uniform_real_distribution<float> uniform_distribution(l,h);

    return uniform_distribution(randomness);
}

double euclidean_distance(Point *p1, Point *p2){
    const vector<int>& Xs_p1 = p1->getXs();
    const vector<int>& Xs_p2 = p2->getXs();

    if(Xs_p1.size() != Xs_p2.size()) exit(1);//Consider removing it for speed

    double sum = 0;

    auto p1_val = Xs_p1.begin();
    auto p2_val = Xs_p2.begin();

    while( p1_val != Xs_p1.end() || p2_val != Xs_p2.end() ){
        sum += (*p2_val - *p1_val)*(*p2_val - *p1_val);
	++p1_val;
	++p2_val;
    }

    return sqrt(sum);
}

// Our own function to calculate dot product of two vector objects
int dot_product(const vector<int> &x,const vector<int> &y){
	if (x.size() != y.size())
		exit(1);
	int n = x.size();
	int dp=0;
	for (int i=0; i<n; i++)
		dp += x[i]*y[i];
	return dp;
}

long int modulo(long int a, long int b){

    long int rem = a % b;
    return (rem<0 ? rem+b : rem);
}

unsigned int randBit(int seed){

    if(seeded == false) { randomness.seed(seed); seeded=true; }
    uniform_int_distribution<int> uniform_distribution(0, 1);

    return uniform_distribution(randomness);
}

unsigned int randUInt(unsigned int l, unsigned int h){

    if(seeded == false) { randomness.seed(time(NULL)); seeded=true; }
    uniform_int_distribution<unsigned int> uniform_distribution(l, h);

    return uniform_distribution(randomness);
}