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
        std::unordered_map<std::string, Point*> queries;
    public:
        FileReader(
            char const *input_f,
            char const *query_f,
            char const *output_f,
            char const *configuration_f = nullptr);
        int getDimension(){ return dimension; }
        Point* ReadPoint(char file='i');
        Point* getQuery(std::string id);
        inline const std::unordered_map<std::string, Point*>& getQueries() {return queries;}
        int writeQuery(const std::string& query_id, PD *knn, PD* bruteForce, int k, double timeLSH, double timeBF, char mode);
        int writeRangeNeighbors(std::unordered_map<std::string, Point*> neighbors);
        int readConfigFile(int &K, int &L, int &k_lsh, int &M, int &k_hc, int &probes);
        int writeClusterPoints(SimpleList *Clusters, int clustering_times, ClusterObject *Medoids, int k, const char* algorithm, bool complete = false);
        int writeClusterPoints(std::unordered_map<std::string, Point*> *Clusters, int clustering_times, ClusterObject *Medoids, int k, const char* algorithm, bool complete = false);
        int writeSilhouette(silhouetteStats *silhouetteS, int k);
        ~FileReader();
};