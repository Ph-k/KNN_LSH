#pragma once

#include <fstream>
#include <vector>

#include "Point.h"

class FileReader{
    private:
        std::ifstream input_file;
        std::ifstream query_file;
        std::ofstream output_file;
        int dimension;
    public:
        FileReader(
            char const *input_f,
            char const *query_f,
            char const *output_f);
        int getDimension(){ return dimension; }
        Point* ReadPoint();
        ~FileReader();
};