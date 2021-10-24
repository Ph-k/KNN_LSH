#include "Hhashing.h"
#include "util.h"
#include <math.h>

#include <iostream>

Hhash::Hhash(std::vector<ItemType> v, int t, int w){
	this->v = v;
	this->t = t;
	this->w = w;
}

int Hhash::hash(std::vector<ItemType> p){
	int h = dot_product(p, v) + t;
	h = floor(h/w);
	return h;
}

Hhash::~Hhash(){}