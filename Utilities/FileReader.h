#pragma once

#include <unordered_map>
#include <fstream>
#include <vector>

#include "Point.h"

#define WORD_SEPERATOR ' '

struct PD; //FD

class FileReader{
    private:
        std::ifstream input_file;
        std::ifstream query_file;
        std::ofstream output_file;
        int dimension;
        int find_dimension_from_input(char const *input_f);
        std::unordered_map<std::string, Point*> queries;
    public:
        FileReader(
            char const *input_f,
            char const *query_f,
            char const *output_f);
        int getDimension(){ return dimension; }
        Point* ReadPoint(char file='i');
        Point* getQuery(std::string id);
        inline const std::unordered_map<std::string, Point*>& getQueries() {return queries;}
        int writeLshQuery(const std::string& query_id, PD *knn, PD* bruteForce, int k, double timeLSH, double timeBF);
        int writeRangeNeighbors(std::unordered_map<std::string, Point*> neighbors);
        std::ofstream& outputStream() {return output_file;}
        ~FileReader();
};