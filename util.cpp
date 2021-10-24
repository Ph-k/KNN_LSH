#include <iostream>
#include <vector>
#include "util.h"

int dot_product(std::vector<int> x, std::vector<int> y){
	if (x.size() != y.size())
		exit(1);
	int n = x.size();
	int dp=0;
	for (int i=0; i<n; i++)
		dp += x[i]*y[i];
	return dp;
}