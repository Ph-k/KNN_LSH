#define ItemType int
#include <iostream>
#include <vector>

class Hhash{
	public:
		Hhash(std::vector<ItemType> v, int t, int w); // requires vector v (picked at random), integer t (random) and param. w
		~Hhash();
		int hash(std::vector<ItemType> p);
	private:
		std::vector<ItemType> v;
		int t;
		int w;
};