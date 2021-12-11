#include "ClusterInterface.h"
#include "HyperCube.h"
#include <vector>

class ClusterHC: public ClusterInterface
{
private:
    inline std::vector<TimeSeries*>* getAllPoints(){return &(HCController->getAllPoints());}
    int M_hc, k_hc, hc_probes, search_range;
    HyperCube *HCController;
    void Assign();
public:
    ClusterHC(FileReader &io_files_ref,int given_k, int M_hc, int k_hc, int hc_probes);
    ~ClusterHC();
};
