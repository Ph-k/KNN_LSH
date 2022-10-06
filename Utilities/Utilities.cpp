#include "FileReader.h"
#include "Utilities.h"
#include "TimeSeries.h"

#include <iostream>
#include <algorithm> // std::max
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

// Create a vector object with floating numbers taken from the normal distribution
vector<double>* uniformD_vec(int dimensions){
    if(seeded == false) { randomness.seed(time(NULL)); seeded=true; }
    normal_distribution<double> normal_distribution(0.0,1.0);

    vector<double> *v = new vector<double>;

    for(int i=0; i<dimensions; i++)
        v->push_back((double)normal_distribution(randomness));

    return v;
}

// Choose random floating point number in the inerval (0,w] from uniform distribution
float random_float(float l, float h){
    if(seeded == false) { randomness.seed(time(NULL)); seeded=true; }
    uniform_real_distribution<float> uniform_distribution(l,h);

    return uniform_distribution(randomness);
}

double euclidean_distance(TimeSeries *p1, TimeSeries *p2){
    const vector<__TIMESERIES_X_TYPE>& Xs_p1 = p1->getXs();
    const vector<__TIMESERIES_X_TYPE>& Xs_p2 = p2->getXs();

    if(Xs_p1.size() != Xs_p2.size()) exit(1);

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

double dfr_distance(TimeSeries *p1, TimeSeries *p2){
    const vector<__TIMESERIES_X_TYPE>& Xs_p1 = p1->getXs();
    const vector<__TIMESERIES_X_TYPE>& Xs_p2 = p2->getXs();

    long unsigned int i,j;
    double **opt = new double*[Xs_p1.size()];
    for(i=0; i<Xs_p1.size(); i++)
        opt[i] = new double[Xs_p2.size()];


    opt[0][0] = abs(Xs_p1.at(0) - Xs_p2.at(0));

    //calculate first row
    j = 1;
    while( j < Xs_p2.size() ){
        opt[0][j] = max(opt[0][j-1],abs(Xs_p1.at(0) - Xs_p2.at(j)) );
        j++;
    }

    //calculate the rest rows of the table
    for(i=1; i<Xs_p1.size(); i++){
        //first cell of row (edge case)
        j = 0;
        opt[i][j] = max(opt[i-1][j],abs(Xs_p1.at(i) - Xs_p2.at(j)) );
        j++;

        //rest cells of row
        while( j < Xs_p2.size() ){
            opt[i][j] = max( 
                min( opt[i-1][j-1], min(opt[i-1][j],opt[i][j-1]) ),
                abs(Xs_p1.at(i) - Xs_p2.at(j))
            );
            j++;
        }
    }

    double ret = opt[Xs_p1.size()-1][Xs_p2.size()-1];

    for(i=0; i<Xs_p1.size(); i++)
        delete[] opt[i];
    delete[] opt;

    return ret;
}

// Our own function to calculate dot product of two vector objects
int dot_product(const vector<double> &x,const vector<double> &y){
    unsigned int i,n;
    if( x.size() <= y.size() ){
        n = x.size();
    }else{
        n = y.size();
    }

	double dp=0;
	for (i=0; i<n; i++)
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

void printPointIdInList(struct PointPointer* pp, void* output_f){
    std::ofstream* output_file = (std::ofstream*)output_f;
    for(auto X: pp->point->getXs())
        *output_file << ' ' << X;
    *output_file << ", ";
}

string algorithmString(const char& method, const char& update){
    string algorithmString("A");
    switch(method){
        case __CLASIC_METHOD:
            algorithmString += "Lloyds";
            break;
        case __LSH_METHOD:
            algorithmString += "LSH";
            break;
        case __LSH_FR_METHOD:
            algorithmString += "LSH_Frechet";
            break;
        case __HC_METHOD:
            algorithmString += "Hypercube";
            break;
        default:
            algorithmString += "x";
    }

    algorithmString += "U";

    switch(update){
        case __MEAN_FR_UPDATE:
            algorithmString += "Mean_Frechet";
            break;
        case __MEAN_VEC_UPDATE:
            algorithmString += "Mean_Vector";
            break;
        default:
            algorithmString += "x";
    }

    return algorithmString;
}