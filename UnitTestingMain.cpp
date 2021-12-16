#include <math.h>
#include <unordered_map>
#include <gtest/gtest.h>

#include "LSH.h"
#include "Utilities.h"
#include "HyperCube.h"
#include "FileReader.h"
#include "TimeSeries.h"

static const int K = 14, L = 3, M = 10, probes = 2;
static const double delta = 5.0;

#define __MY_KNN_TEST_INIT FileReader io_files("./UnitTestingInput","./UnitTestingInputQuery","./output.UnitTesting"); \
    PD *brute_force = nullptr; \
    const std::unordered_map<std::string, TimeSeries*> &queries = io_files.getQueries();

TEST(Lsh, ClassicTest){
    __MY_KNN_TEST_INIT

    LSH lsh(io_files,150,K,L,delta,1000, __LSH_MODE);
    lsh.bruteForceNN(queries.begin()->second,L,1,&brute_force);
    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}


TEST(Lsh, FRECHET_DISCRETE){
    __MY_KNN_TEST_INIT

    LSH lsh(io_files,150,K,L,delta,1000, __FRECHET_DISCRETE_MODE);
    lsh.bruteForceNN(queries.begin()->second,L,1,&brute_force);
    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}

TEST(Lsh,FRECHET_CONTINUOUS){
    __MY_KNN_TEST_INIT

    LSH lsh(io_files,150,K,L,delta,1000, __FRECHET_CONTINUOUS_MODE);
    lsh.bruteForceNN(queries.begin()->second,L,1,&brute_force);
    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}

TEST(HyperCube,HyperCube){
    __MY_KNN_TEST_INIT

    HyperCube HC(io_files,150,K,M,probes,pow(2,K));
    HC.bruteForceNN(queries.begin()->second,L,1,&brute_force);

    EXPECT_EQ (0.0, euclidean_distance(brute_force[0].p,queries.begin()->second));

    if(brute_force != nullptr) delete[] brute_force;
}

/*TEST (SquareRootTest, ZeroAndNegativeNos) { 
    ASSERT_EQ (0.0, square (0.0));
}*/

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}