#pragma once

#include <vector>

#define __LSH_MODE 0
#define __H_CUBE_MODE 1

class Point;

struct PD{
    Point *p;
    double distance;
};

struct PointPointer{
    Point *point;
    int Id;
};

std::vector<int>* uniform_vec(int dimensions);

float random_float(float l=2.0, float h=6.0);

double euclidean_distance(Point *p1, Point *p2);

int dot_product(const std::vector<int> &x,const std::vector<int> &y);

long int modulo(long int a, long int b);

unsigned int randBit(int seed = time(NULL));

unsigned int randUInt(unsigned int l, unsigned int h);

int binarySearch(std::vector<double> &P);

void printPointIdInList(struct PointPointer* pp, void* output_f);