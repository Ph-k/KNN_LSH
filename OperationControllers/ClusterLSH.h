#include "ClusterInterface.h"
#include "LSH.h"
#include <vector>

class ClusterLSH: public ClusterInterface
{
private:
    inline std::vector<TimeSeries*>* getAllPoints(){return &(LSHController->getAllPoints());}
    int k_lsh, l_lsh, search_range;
    LSH *LSHController;
    void Assign();
public:
    ClusterLSH(FileReader &io_files_ref,int given_k, double (*metric_func)(ClusterObject, ClusterObject),int k_lsh, int l_lsh,char update_metric);
    ~ClusterLSH();
};