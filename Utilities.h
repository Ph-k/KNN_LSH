#pragma once

#include <vector>
#include <climits> // For INT_MAX

std::vector<int>* uniform_vec(int dimensions);

float random_float(int w = INT_MAX);

int dot_product(const std::vector<int> &x,const std::vector<int> &y);
