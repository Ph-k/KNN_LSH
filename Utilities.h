#pragma once

#include <vector>
#include <climits> // For INT_MAX

class Point;

struct PD{
    Point *p;
    float distance;
};

struct PointPointer{
    Point *point;
    int Id;
};

std::vector<int>* uniform_vec(int dimensions);

float random_float(int w = INT_MAX);

float euclidean_distance(Point *p1, Point *p2);

int dot_product(const std::vector<int> &x,const std::vector<int> &y);
