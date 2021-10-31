#include <iostream>
#include <string.h>
#include <unordered_map>

#include "LSH.h"
#include "Point.h"
#include "Utilities.h"


using namespace std;

int main(int argc, char const *argv[]){

    int i, K=-1, L=-1, number_of_nearest=-1, radius=-1;
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
		}else if( strcmp(argv[i],"-L") == 0){
			L = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-N") == 0){
			number_of_nearest = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-R") == 0){
			radius = atoi(argv[++i]);
		}else{
            cout << "Argument '" << argv[i] << "' is invalid!" << endl;
        }
	}

    if(input_file == nullptr || query_file == nullptr || output_file == nullptr){
        cout << "lsh: At least one of the input_file, query_file, output_file was mistyped or not given at all!" << endl;
        return -1;
    }

    if( K == -1){
        cout << "lsh: using default value for argument K" << endl;
        K = 14;
    }

    if( L == -1 ){
        cout << "lsh: using default value for argument L" << endl;
        L = 5;
    }

    if( number_of_nearest == -1 ){
        cout << "lsh: using default value for argument N" << endl;
        number_of_nearest = 1;
    }

    if( radius == -1 ){
        cout << "lsh: using default value for argument R" << endl;
        radius = 10000;
    }

    cout << "Argumets:"
         << "\n\tinput_file: " << input_file
         << "\n\tquery_file: " << query_file
         << "\n\toutput_file: " << output_file
         << "\n\tK: " << K
         << "\n\tL: " << L
         << "\n\tnumber_of_nearest: " << number_of_nearest
         << "\n\tradius: " << radius << endl;


    FileReader io_files(input_file,query_file,output_file);
    LSH operations(io_files,300,K,L,1000);

    double time_lsh, time_brute_force;
    PD *knn = nullptr, *brute_force = nullptr;
    unordered_map<string, Point*> r_neighbors;
    for(i = 1; i <=100; i++){
        string id = to_string(i);

        time_lsh = operations.kNN_Search(id,L,K,&knn);        

        time_brute_force = operations.bruteForceNN(id,L,K,&brute_force);

        io_files.writeLshQuery(id, knn, brute_force, K, time_lsh, time_brute_force);

        operations.rangeSearch(id, radius, r_neighbors);

        io_files.writeRangeNeighbors(r_neighbors);
    }
    if(knn != nullptr) delete[] knn;
    if(brute_force != nullptr) delete[] brute_force;

    return 0;
}
