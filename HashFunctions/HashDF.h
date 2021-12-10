#include <vector>
#include <TimeSeries.h>

class HashDF{
    private:
        double *t;
        double delta;
        int d;
	public:
		HashDF(double given_delta, int given_d);
		~HashDF();
		std::vector<__TIMESERIES_X_TYPE> *Snap(const std::vector<__TIMESERIES_X_TYPE> &p); // h_delta function
};
