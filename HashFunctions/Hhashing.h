#pragma once

#include <iostream>
#include <vector>

#define ItemType double

class Hhash{
	public:
		Hhash(std::vector<ItemType> *v, float t, int w); // requires vector v (picked at random), integer t (random) and param. w
		~Hhash();
		int hash(const std::vector<ItemType> &p);
	private:
		std::vector<ItemType> *v;
		float t;
		int w;
};