#include <iostream>
#include <string.h>
#include <unordered_map>

#include "Point.h"
#include "Utilities.h"
#include "FileReader.h"
#include "ClusterComplex.h"

using namespace std;

int main(int argc, char const *argv[]){

    int i, K, L, k_lsh, M_hc, k_hc, hc_probes;
    char const *input_file  = nullptr,
               *query_file = nullptr,
               *output_file = nullptr,
               *configuration_file = nullptr,
               *method_string = nullptr;
    char method = __INVALID_METHOD;
    bool complete = false;

	for(i=1; i<argc; i++){
		if( strcmp(argv[i], "-i") == 0 ){
			input_file = argv[++i];
		}else if( strcmp(argv[i],"-q") == 0 ){
			query_file = argv[++i];
		}else if( strcmp(argv[i],"-o") == 0 ){
			output_file = argv[++i];
		}else if( strcmp(argv[i],"-c") == 0){
			configuration_file = argv[++i];
		}else if( strcmp(argv[i],"-complete") == 0){
			complete = true;
		}else if( strcmp(argv[i],"-m") == 0){
			method_string = argv[++i];

            if( strcmp(method_string,"Classic") == 0){
                method = __CLASIC_METHOD;
            }else if( strcmp(method_string,"LSH") == 0){
                method = __LSH_METHOD;
            }else if( strcmp(method_string,"Hypercube") == 0){
                method = __HC_METHOD;
            }

		}else{
            cout << "Argument '" << argv[i] << "' is invalid!" << endl;
        }
	}

    if(input_file == nullptr || query_file == nullptr || output_file == nullptr || configuration_file == nullptr){
        cout << "cluster: At least one of the input_file, query_file, output_file, configuration_file was mistyped or not given at all!" << endl;
        return -1;
    }

    if(method == __INVALID_METHOD){
        cout << "cluster: method argument (-m) was misstyped or not given at all, defaulting to Classic" << endl;
        method = __CLASIC_METHOD;
    }

    FileReader io_files(input_file,query_file,output_file,configuration_file);

    cout << "Argumets:"
         << "\n\tinput_file: " << input_file
         << "\n\tquery_file: " << query_file
         << "\n\toutput_file: " << output_file
         << "\n\tconfiguration_file: " << configuration_file
         << "\n\tmethod: " << (method_string != nullptr? method_string : "default" ) << '\n';

    if( io_files.readConfigFile(K, L, k_lsh, M_hc, k_hc, hc_probes) != 0 ) return -3;

    cout << "Configuration file argumets:"
         << "\n\tnumber_of_clusters: " << K
         << "\n\tnumber_of_vector_hash_tables: " << L
         << "\n\tnumber_of_vector_hash_functions: " << k_lsh
         << "\n\tmax_number_M_hypercube: " << M_hc
         << "\n\tnumber_of_hypercube: " << k_hc
         << "\n\tnumber_of_probes: " << hc_probes << endl;


    ClusterComplex *clustering;
    switch(method){
        case __CLASIC_METHOD:
            clustering = new ClusterComplex(io_files,K,method);
            break;
        case __LSH_METHOD:
            clustering = new ClusterComplex(io_files,K,method,k_lsh,L);
            break;
        case __HC_METHOD:
            clustering = new ClusterComplex(io_files,K,method,M_hc,k_hc,hc_probes);
            break;
        default:
            return -3;
    }

    clustering->kMeans(10);

    io_files.writeClusterPoints(clustering->getClusters(1), clustering->getMedoids(), K);

    /*double time_cube, time_brute_force;
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
    if(brute_force != nullptr) delete[] brute_force;*/

    delete clustering;
    return 0;
}