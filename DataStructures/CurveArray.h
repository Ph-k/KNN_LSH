#include <unordered_map>
#include <string>

class TimeSeries;

class CurveArray{
    private:
        TimeSeries** array;
        unsigned int arrSize;
        std::unordered_map<std::string, TimeSeries*> &Cluster;
        void recursivePostOrderTraversal(unsigned int node_index);
    public:
        CurveArray(std::unordered_map<std::string, TimeSeries*> &gCluster);
        TimeSeries *postOrderTraversal();
        ~CurveArray();
};