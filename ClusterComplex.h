#include "Point.h"
#include "DataStructures/SimpleList.h"
#include "Utilities/Utilities.h"

#define ClusterObject Point*

class ClusterComplex
{
private:
    int k;
    ClusterObject *Medoids;
    SimpleList *Clusters;
    int *T;
public:
    ClusterComplex(int k, std::vector<ClusterObject> medoids, std::vector<ClusterObject> nonMedoids);
    ~ClusterComplex();
    // void InitializeCenters(std::vector<ClusterObject> medoids);
    void Insert(ClusterObject item);
    int nearestCenter(ClusterObject item);
    void Update();
};

