#include <vector>
#include "Point.h"
#include "HashTable.h"
#include "Ghashing.h"

struct PD{
	Point *p;
	float distance;
};

PD **kNN_Search(Point *q, HashTable *TableArray, int L, int k){

	PD **b = new *PD[k];
	int i;
	for (i=0; i<L; i++){
		b = TableArray[i].kNN_Search(k, b);
	}
	return b;

}