#include <math.h>
#include <iostream>
#include <string.h>
#include <unordered_map>

#include "Point.h"
#include "Utilities.h"
#include "FileReader.h"
#include "HyperCube.h"

using namespace std;

int main(int argc, char const *argv[]){

    int i, K=-1, M=-1, probes=-1, number_of_nearest=-1, radius=-1;
    char const *input_file  = nullptr, *query_file = nullptr, *output_file = nullptr;

	for(i=1; i<argc; i++){
		if( strcmp(argv[i], "-i") == 0 ){
			input_file = argv[++i];
		}else if( strcmp(argv[i],"-q") == 0 ){
			query_file = argv[++i];
		}else if( strcmp(argv[i],"-o") == 0 ){
			output_file = argv[++i];
		}else if( strcmp(argv[i],"-k") == 0){
			K = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-M") == 0){
			M = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-probes") == 0){
			probes = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-N") == 0){
			number_of_nearest = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-R") == 0){
			radius = atoi(argv[++i]);
		}else{
            cout << "Argument '" << argv[i] << "' is invalid!" << endl;
        }
	}

    if(input_file == nullptr || query_file == nullptr || output_file == nullptr){
        cout << "cube: At least one of the input_file, query_file, output_file, configuration_file was mistyped or not given at all!" << endl;
        return -1;
    }

    if( K == -1){
        cout << "cube: using default value for argument K" << endl;
        K = 14;
    }

    if( M == -1 ){
        cout << "cube: using default value for argument M" << endl;
        probes = 5;
    }

    if( probes == -1 ){
        cout << "cube: using default value for argument probes" << endl;
        probes = 5;
    }

    if( number_of_nearest == -1 ){
        cout << "cube: using default value for argument N" << endl;
        number_of_nearest = 1;
    }

    if( radius == -1 ){
        cout << "cube: using default value for argument R" << endl;
        radius = 10000;
    }

    FileReader io_files(input_file,query_file,output_file);
    HyperCube HC(io_files,150,K,M,probes,pow(2,K));

    cout << "Argumets:"
         << "\n\tinput_file: " << input_file
         << "\n\tquery_file: " << query_file
         << "\n\toutput_file: " << output_file
         << "\n\tK: " << K
         << "\n\tM: " << M
         << "\n\tprobes: " << probes
         << "\n\tnumber_of_nearest: " << number_of_nearest
         << "\n\tradius: " << radius << endl;

    double time_cube, time_brute_force;
    PD *knn = nullptr, *brute_force = nullptr;
    unordered_map<string, Point*> r_neighbors;
    for(i = 1; i <=100; i++){
        string id = to_string(i);

        time_cube = HC.kNN_Search(id,-1,K,&knn);        

        time_brute_force = HC.bruteForceNN(id,-1,K,&brute_force);

        io_files.writeQuery(id, knn, brute_force, K, time_cube, time_brute_force,__H_CUBE_MODE);

        HC.rangeSearch(id, radius, r_neighbors);

        io_files.writeRangeNeighbors(r_neighbors);
    }
    if(knn != nullptr) delete[] knn;
    if(brute_force != nullptr) delete[] brute_force;

    return 0;
}