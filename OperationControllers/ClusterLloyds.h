#include "ClusterInterface.h"

class ClusterLloyds: public ClusterInterface
{
private:
     std::vector<ClusterObject> points;
     void Assign();
     inline std::vector<TimeSeries*>* getAllPoints(){return &(this->points);}
public:
    ClusterLloyds(FileReader &io_files_ref,int given_k);
    ~ClusterLloyds();
};

