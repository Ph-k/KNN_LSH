#include <iostream>
#include <string.h>
#include <unordered_map>

#include "TimeSeries.h"
#include "Utilities.h"
#include "FileReader.h"
#include "ClusterLloyds.h"
#include "ClusterLSH.h"
#include "ClusterHC.h"

static int const epochs = 10;

using namespace std;

int main(int argc, char const *argv[]){

    int i, K, L, k_lsh, M_hc, k_hc, hc_probes;
    char const *input_file  = nullptr,
               *output_file = nullptr,
               *configuration_file = nullptr,
               *method_string = nullptr,
               *update_string = nullptr;
    char method = __INVALID_METHOD, update = __INVALID_UPDATE;
    bool complete = false, silhouette = false;

	for(i=1; i<argc; i++){
		if( strcmp(argv[i], "-i") == 0 ){
			input_file = argv[++i];
		}else if( strcmp(argv[i],"-o") == 0 ){
			output_file = argv[++i];
		}else if( strcmp(argv[i],"-c") == 0){
			configuration_file = argv[++i];
		}else if( strcmp(argv[i],"-complete") == 0){
			complete = true;
		}else if( strcmp(argv[i],"-update") == 0){
            update_string = argv[++i];

			if( strcmp(update_string,"Mean_Frechet") == 0){
                update = __MEAN_FR_UPDATE;
            }else if( strcmp(update_string,"Mean_Vector") == 0){
                update = __MEAN_VEC_UPDATE;
            }

		}else if( strcmp(argv[i],"-assignment") == 0){
			method_string = argv[++i];

            if( strcmp(method_string,"Classic") == 0){
                method = __CLASIC_METHOD;
            }else if( strcmp(method_string,"LSH") == 0){
                method = __LSH_METHOD;
            }else if( strcmp(method_string,"LSH_Frechet") == 0){
                method = __LSH_FR_METHOD;
            }else if( strcmp(method_string,"Hypercube") == 0){
                method = __HC_METHOD;
            }

		}else if( strcmp(argv[i],"-silhouette") == 0){
			silhouette = true;
		}else{
            cout << "Argument '" << argv[i] << "' is invalid!" << endl;
        }
	}

    if(input_file == nullptr || output_file == nullptr || configuration_file == nullptr){
        cout << "cluster: At least one of the input_file, output_file, configuration_file was mistyped or not given at all!" << endl;
        return -1;
    }

    if(method == __INVALID_METHOD){
        cout << "cluster: method argument (-assignment) was misstyped or not given at all, defaulting to Classic" << endl;
        method = __CLASIC_METHOD;
    }

    if(update == __INVALID_UPDATE){
        cout << "cluster: method argument (-update) was misstyped or not given at all, defaulting to Mean frechet" << endl;
        update = __MEAN_FR_UPDATE;
    }

    FileReader io_files(input_file,nullptr,output_file,configuration_file);

    cout << "Argumets:"
         << "\n\tinput_file: " << input_file
         << "\n\toutput_file: " << output_file
         << "\n\tconfiguration_file: " << configuration_file
         << "\n\tassignment: " << (method_string != nullptr? method_string : "default" )
         << "\n\tupdate: " << (update_string != nullptr? update_string : "default" ) << '\n';

    if( io_files.readConfigFile(K, L, k_lsh, M_hc, k_hc, hc_probes) != 0 ) return -3;

    cout << "Configuration file argumets:"
         << "\n\tnumber_of_clusters: " << K
         << "\n\tnumber_of_vector_hash_tables: " << L
         << "\n\tnumber_of_vector_hash_functions: " << k_lsh
         << "\n\tmax_number_M_hypercube: " << M_hc
         << "\n\tnumber_of_hypercube_dimensions: " << k_hc
         << "\n\tnumber_of_probes: " << hc_probes << endl;


    ClusterInterface *clustering;
    switch(method){
        case __CLASIC_METHOD:
            clustering = new ClusterLloyds(io_files,K,update);
            break;
        case __LSH_METHOD:
            clustering = new ClusterLSH(io_files,K,&euclidean_distance,k_lsh,L,update);
            break;
        case __LSH_FR_METHOD:
            clustering = new ClusterLSH(io_files,K,&dfr_distance,k_lsh,L,update);
            break;
        case __HC_METHOD:
            clustering = new ClusterHC(io_files,K,M_hc,k_hc,hc_probes);
            break;
        default:
            return -3;
    }

    clustering->kMeans(epochs);

    io_files.writeClusterPoints(clustering->getClusters(), clustering->getClusteringTimes() ,clustering->getMedoids(), K, algorithmString(method,update).c_str() , complete);

    if(silhouette)
        io_files.writeSilhouette(clustering->Silhouette(),K);

    delete clustering;
    return 0;
}