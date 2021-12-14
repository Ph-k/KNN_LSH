#include <math.h>
#include <iostream>
#include <string.h>
#include <unordered_map>

#include "LSH.h"
#include "HyperCube.h"
#include "TimeSeries.h"
#include "Utilities.h"


using namespace std;

int main(int argc, char const *argv[]){

    int i, K=-1, L=-1, number_of_nearest=-1, radius=-1, M=-1, probes=-1;
    double delta = -1.0;
    char method=-1;
    char const *input_file  = nullptr, *query_file = nullptr, *output_file = nullptr, *method_string = nullptr, *frechet_method_string = nullptr;

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
		}else if( strcmp(argv[i],"-M") == 0){
			M = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-probes") == 0){
			probes = atoi(argv[++i]);
		}else if( strcmp(argv[i],"-delta") == 0){
			delta = atof(argv[++i]);
		}else if( strcmp(argv[i],"-algorithm") == 0){
			method_string = argv[++i];

            if( strcmp(method_string,"LSH") == 0){
                method = __LSH_MODE;
            }else if( strcmp(method_string,"Hypercube") == 0){
                method = __H_CUBE_MODE;
            }else if( strcmp(method_string,"Frechet") == 0){
                method = -1;

                if( strcmp(argv[++i],"-metric") == 0){
                    frechet_method_string = argv[++i];
			        if( strcmp(frechet_method_string,"continuous") == 0){
                        method = __FRECHET_CONTINUOUS_MODE;
                    }else if( strcmp(frechet_method_string,"discrete") == 0){
                        method = __FRECHET_DISCRETE_MODE;
                    }
                }
                
            }

		}else{
            cout << "Argument '" << argv[i] << "' is invalid!" << endl;
        }
	}

    if(method == -1){
        cout << "search: no method specified!" << endl;
        return -1;
    }

    if(input_file == nullptr || query_file == nullptr || output_file == nullptr){
        cout << "search: At least one of the input_file, query_file, output_file was mistyped or not given at all!" << endl;
        return -1;
    }

    if( K == -1){
        cout << "search: using default value for argument K" << endl;
        K = 14;
    }

    if( number_of_nearest == -1 ){
        cout << "search: using default value for argument N" << endl;
        number_of_nearest = 1;
    }

    if( radius == -1 ){
        cout << "search: using default value for argument R" << endl;
        radius = 10000;
    }

    if( method == __LSH_MODE && L == -1 ){
        cout << "search: using default value for argument L" << endl;
        L = 3;
    }

    if( method == __H_CUBE_MODE ){
        if( M == -1){
            cout << "search: using default value for argument M" << endl;
            M = 10;
        }
        if( probes == -1){
            cout << "search: using default value for argument probes" << endl;
            probes = 2;
        }
    }


    if( method == -1 ){
        cout << "search: Frechet algorithm requested, but -metric argument is missing!, defaulting to discrete" << endl;
        method = __FRECHET_DISCRETE_MODE;
        frechet_method_string = "N/A";
    }

    if( method == __FRECHET_DISCRETE_MODE || method == __FRECHET_CONTINUOUS_MODE ){
        if(delta == -1.0){
            cout << "search: using default value for argument delta" << endl;
            delta = 5.0;
        }
    }

    cout << "Argumets:"
         << "\n\tinput_file: " << input_file
         << "\n\tquery_file: " << query_file
         << "\n\toutput_file: " << output_file
         << "\n\tK: " << K
         << "\n\tnumber_of_nearest: " << number_of_nearest
         << "\n\tradius: " << radius
         << "\n\talgorithm: " << method_string;

    if(method == __LSH_MODE){
        cout << "\n\tL: " << L;
    }else if(method == __H_CUBE_MODE){
        cout << "\n\tM: " << M;
        cout << "\n\tprobes: " << probes;
    }else if( method == __FRECHET_DISCRETE_MODE || method == __FRECHET_CONTINUOUS_MODE ){
        cout << "\n\tfrechet method : " << frechet_method_string;
        cout << "\n\tdelta: " << delta;
    } cout << endl;

    FileReader io_files(input_file,query_file,output_file);

    MappingMethod* operations;

    if(method == __LSH_MODE || method == __FRECHET_DISCRETE_MODE || method == __FRECHET_CONTINUOUS_MODE){
        operations = new LSH(io_files,150,K,L,delta,1000, method);
    }else if(method == __H_CUBE_MODE){
        operations = new HyperCube(io_files,150,K,M,probes,pow(2,K));
    }


    unsigned int index = 0;
    long double time_brute_force = 0.0;
    double time_aprx = 0.0, MAF = -1.0;
    PD *knn = nullptr, *brute_force = nullptr;
    unordered_map<string, TimeSeries*> r_neighbors;

    const unordered_map<string, TimeSeries*> &queries = io_files.getQueries();
    for(auto &query: queries){

        time_aprx += nanosecToMilliSec(operations->kNN_Search(L,1,&knn,query.second));

        time_brute_force += nanosecToMilliSec(operations->bruteForceNN(query.second,L,1,&brute_force));

        io_files.writeQuery(query.first, knn, brute_force, 1, method,MAF);

        /*operations->rangeSearch(radius, r_neighbors, query.second);

        io_files.writeRangeNeighbors(r_neighbors);*/
        index++;
    }

    io_files.writeQueryTimes(time_aprx, time_brute_force, index, MAF);

    if(knn != nullptr) delete[] knn;
    if(brute_force != nullptr) delete[] brute_force;
    delete operations;

    return 0;
}
