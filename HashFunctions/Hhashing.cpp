#include "Hhashing.h"
#include "Utilities.h"
#include <math.h>

Hhash::Hhash(std::vector<ItemType> *v, float t, int w)
:v(v), t(t), w(w) {}

int Hhash::hash(const std::vector<ItemType> &p){
	int h = dot_product(p, *v) + t;
	h = floor(h/w);
	return h;
}

Hhash::~Hhash(){
	if(v!=nullptr) delete v;
}