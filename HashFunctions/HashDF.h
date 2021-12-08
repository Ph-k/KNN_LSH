#include <vector>

class HashDF{
    private:
        static double *baseGrid;
        double *Grid;
        double delta;
        int d;
	public:
		HashDF(double delta, int d);
		~HashDF();
		std::vector<double> *Snap(const std::vector<double> &p); // h_delta function
};
