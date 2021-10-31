#pragma once

#include <vector>

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
