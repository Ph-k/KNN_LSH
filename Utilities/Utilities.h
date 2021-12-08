#pragma once

#include <vector>


#define __LSH_MODE 0
#define __H_CUBE_MODE 1
#define __FRECHET_MODE 2

#define __FRECHET_DISCRETE_MODE 0
#define __FRECHET_CONTINUOUS_MODE 1

class TimeSeries;

struct PD{
    TimeSeries *p;
    double distance;
};

struct PointPointer{
    TimeSeries *point;
    int Id;
};

std::vector<int>* uniform_vec(int dimensions);

float random_float(float l=2.0, float h=6.0);

double euclidean_distance(TimeSeries *p1, TimeSeries *p2);

int dot_product(const std::vector<int> &x,const std::vector<int> &y);

long int modulo(long int a, long int b);

unsigned int randBit(int seed = time(NULL));

unsigned int randUInt(unsigned int l, unsigned int h);

void printPointIdInList(struct PointPointer* pp, void* output_f);

inline double nanosecToMilliSec(int nanosec){ return ((double)nanosec)/1000000.0; }

inline double millisecToSec(int nanosec){ return ((double)nanosec)/1000.0; }