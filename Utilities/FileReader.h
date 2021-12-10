#pragma once

#include <unordered_map>
#include <fstream>
#include <vector>

#include "ClusterComplex.h"

#define WORD_SEPERATOR ' '

struct PD; //FD
struct silhouetteStats;

class FileReader{
    private:
        std::ifstream input_file;
        std::ifstream query_file;
        std::ofstream output_file;
        std::ifstream configuration_file;
        int dimension;
        int find_dimension_from_input(char const *input_f);
        std::unordered_map<std::string, TimeSeries*> queries;
    public:
        FileReader(
            char const *input_f,
            char const *query_f,
            char const *output_f,
            char const *configuration_f = nullptr);
        int getDimension(){ return dimension; }
        TimeSeries* ReadPoint(char file='i');
        TimeSeries* getQuery(std::string id);
        inline const std::unordered_map<std::string, TimeSeries*>& getQueries() {return queries;}
        int writeQuery(const std::string& query_id, PD *knn, PD* bruteForce, int k, char mode, char fr_mode = -1);
        int writeQueryTimes(double timeAprx, double timeBF, int count);
        int writeRangeNeighbors(std::unordered_map<std::string, TimeSeries*> neighbors);
        int readConfigFile(int &K, int &L, int &k_lsh, int &M, int &k_hc, int &probes);
        int writeClusterPoints(SimpleList *Clusters, int clustering_times, ClusterObject *Medoids, int k, const char* algorithm, bool complete = false);
        int writeClusterPoints(std::unordered_map<std::string, TimeSeries*> *Clusters, int clustering_times, ClusterObject *Medoids, int k, const char* algorithm, bool complete = false);
        int writeSilhouette(silhouetteStats *silhouetteS, int k);
        ~FileReader();
};