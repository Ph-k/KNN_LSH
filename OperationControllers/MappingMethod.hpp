#pragma once

#include <unordered_map>
#include "FileReader.h"
#include "HashTable.h"

class MappingMethod{
    protected:
        FileReader &io_files;
        std::vector<TimeSeries*> points;
    public:
        MappingMethod(FileReader &io_files_ref);
        virtual int kNN_Search(int L, int k, PD **b, std::string &id) =0;
        virtual int kNN_Search(int L, int k, PD **b, TimeSeries *q) =0;
        virtual int bruteForceNN(std::string &id, int L, int k, PD **b) =0;
        virtual int bruteForceNN(TimeSeries *q, int L, int k, PD **b) =0;
        virtual int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, std::string &id) =0;
        virtual int rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, TimeSeries *q) =0;
        inline std::vector<TimeSeries*>& getAllPoints(){ return this->points; }
        virtual ~MappingMethod();
};