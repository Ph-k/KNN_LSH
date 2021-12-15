#include <unordered_map>
#include <string>

class TimeSeries;

class CurveArray{
    private:
        TimeSeries** array;
        std::unordered_map<std::string, TimeSeries*> &Cluster;
    public:
        CurveArray(std::unordered_map<std::string, TimeSeries*> &gCluster);
        TimeSeries *postOrderTraversal();
        ~CurveArray();
};