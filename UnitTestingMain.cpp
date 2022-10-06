#include <math.h>
#include <unordered_map>
#include <gtest/gtest.h>

#include "LSH.h"
#include "Utilities.h"
#include "HyperCube.h"
#include "ClusterHC.h"
#include "FileReader.h"
#include "TimeSeries.h"
#include "ClusterLSH.h"
#include "ClusterLloyds.h"

#define __INPUT_FILE "./UnitTestingInput"
#define __QUERY_FILE "./UnitTestingInputQuery"
#define __OUTPUT_FILE "./output.UnitTesting"

static const int K = 14, L = 3, M = 10, probes = 2, epochs = 10, K_cluster = 3, M_hc = 10, k_hc = 3, hc_probes = 2;
static const double delta = 5.0;

#define __MY_KNN_TEST_INIT FileReader io_files(__INPUT_FILE,__QUERY_FILE,__OUTPUT_FILE); \
    PD *brute_force = nullptr; \
    const std::unordered_map<std::string, TimeSeries*> &queries = io_files.getQueries();

TEST(KNN, Classic_LSH){
    __MY_KNN_TEST_INIT

    LSH lsh(io_files,150,K,L,delta,1000, __LSH_MODE);
    lsh.bruteForceNN(queries.begin()->second,L,1,&brute_force);
    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}


TEST(KNN, FRECHET_DISCRETE){
    __MY_KNN_TEST_INIT

    LSH lsh(io_files,150,K,L,delta,1000, __FRECHET_DISCRETE_MODE);
    lsh.bruteForceNN(queries.begin()->second,L,1,&brute_force);
    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}

TEST(KNN,FRECHET_CONTINUOUS){
    __MY_KNN_TEST_INIT

    LSH lsh(io_files,150,K,L,delta,1000, __FRECHET_CONTINUOUS_MODE);
    lsh.bruteForceNN(queries.begin()->second,L,1,&brute_force);
    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}

TEST(KNN,Hyper_Cube){
    __MY_KNN_TEST_INIT

    HyperCube HC(io_files,150,K,M,probes,pow(2,K));
    HC.bruteForceNN(queries.begin()->second,L,1,&brute_force);

    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}

TEST(Clustering,Lloyds){
    FileReader *io_files;
    ClusterInterface *clustering;
    unsigned int clustered_items;

    io_files = new FileReader(__INPUT_FILE,nullptr,__OUTPUT_FILE);
    clustering = new ClusterLloyds(*io_files,K_cluster,__MEAN_VEC_UPDATE);
    clustering->kMeans(epochs);
    clustered_items = 0;
    for(int i=0; i<K_cluster; i++)
        clustered_items += clustering->getClusters()[i].size();

    EXPECT_EQ (9 , clustered_items);

    delete clustering;
    delete io_files;

    io_files = new FileReader(__INPUT_FILE,nullptr,__OUTPUT_FILE);
    clustering = new ClusterLloyds(*io_files,K_cluster,__MEAN_FR_UPDATE);
    clustering->kMeans(epochs);
    clustered_items = 0;
    for(int i=0; i<K_cluster; i++)
        clustered_items += clustering->getClusters()[i].size();

    EXPECT_EQ (9 , clustered_items);

    delete clustering;
    delete io_files;

}

TEST(Clustering,LSH){
    FileReader *io_files;
    ClusterInterface *clustering;
    unsigned int clustered_items;

    io_files = new FileReader(__INPUT_FILE,nullptr,__OUTPUT_FILE);
    clustering = new ClusterLSH(*io_files,K_cluster,&euclidean_distance,K,L,__MEAN_VEC_UPDATE);
    clustering->kMeans(epochs);
    clustered_items = 0;
    for(int i=0; i<K_cluster; i++)
        clustered_items += clustering->getClusters()[i].size();

    EXPECT_EQ (9 , clustered_items);
    delete clustering;
    delete io_files;

    io_files = new FileReader(__INPUT_FILE,nullptr,__OUTPUT_FILE);
    clustering = new ClusterLSH(*io_files,K_cluster,&dfr_distance,K,L,__MEAN_FR_UPDATE);
    clustering->kMeans(epochs);
    clustered_items = 0;
    for(int i=0; i<K_cluster; i++)
        clustered_items += clustering->getClusters()[i].size();

    EXPECT_EQ (9 , clustered_items);
    delete clustering;
    delete io_files;

}

TEST(Clustering,Hyper_Cube){
    FileReader io_files(__INPUT_FILE,nullptr,__OUTPUT_FILE);
    ClusterHC clustering(io_files,K_cluster,M_hc,k_hc,hc_probes);
    unsigned int clustered_items;

    clustering.kMeans(epochs);
    clustered_items = 0;
    for(int i=0; i<K_cluster; i++)
        clustered_items += clustering.getClusters()[i].size();

    EXPECT_EQ (9 , clustered_items);

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}