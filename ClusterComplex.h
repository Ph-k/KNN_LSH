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
    void Update();
    void Assign(std::vector<ClusterObject> &points);
public:
    ClusterComplex(int k, std::vector<ClusterObject> &medoids, std::vector<ClusterObject> &nonMedoids);
    ~ClusterComplex();
    void Insert(ClusterObject item);
    int nearestCenter(ClusterObject item);
    void kMeans(std::vector<ClusterObject> &points, int epochs);
    inline SimpleList *getClusters(){return Clusters;};
    inline ClusterObject *getMedoids(){return Medoids;};
};

