#include "FileReader.h"
#include "Utilities.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

FileReader::FileReader(char const *input_f, char const *query_f, char const *output_f, char const *configuration_f)
{
    input_file.open(input_f);
    if( input_file.is_open() == false )
        {cout << "ERROR: could not open input file: " << input_f << endl; exit(-1);}

	if(query_f != nullptr){
		query_file.open(query_f);
		if( query_file.is_open() == false )
			{cout << "ERROR: could not open query file: " << query_f << endl; exit(-1);}
	}

    output_file.open(output_f);
    if( output_file.is_open() == false )
        {cout << "ERROR: could not create ouput file: " << output_f << endl; exit(-1);}

    if(configuration_f != nullptr){
        configuration_file.open(configuration_f);
        if( configuration_file.is_open() == false )
            {cout << "ERROR: could not create configuration file: " << configuration_f << endl; exit(-1);}
    }

    dimension = this->find_dimension_from_input(input_f);
    if(dimension == -1) 
        {cout << "Error: invalid file format!" << endl; exit(-1);}


    TimeSeries *p = ReadPoint('q');
    while( p != nullptr ){
        pair<string, TimeSeries*> temp(p->getId(),p);
        queries.insert( temp );
        p = ReadPoint('q');
    }
}

int FileReader::find_dimension_from_input(char const *input_f){
    ifstream input_fs;
    input_fs.open(input_f);

    string line,word;
    if ( input_file.peek() != EOF ) 
        getline(input_file, line);
    else
        return -1;

    istringstream line_stream(line);

    int s=0;

    while(line_stream >> word)
        s++;

    return s-1;
}

TimeSeries* FileReader::ReadPoint(char file_mode){

    ifstream *file = nullptr;
    switch (file_mode){
        case 'i':
            file = &(this->input_file);
            break;
        case 'q':
			if( query_file.is_open() == false ) return nullptr;
            file = &(this->query_file);
            break;
        default:
            return nullptr;
    } 

    string line;
    if ( file->peek() != EOF ) 
        getline(*file, line);
    else
        return nullptr;

    istringstream line_stream(line);

    vector<__TIMESERIES_X_TYPE> *given_Xs = new vector<__TIMESERIES_X_TYPE>;

    double Xnum;
    string Xstring, *item_id = new string;
    line_stream >> *item_id;
    while (line_stream >> Xstring){

        try {
            Xnum = stod(Xstring);
        }catch(...) {
            cout << "Error: invalid input file format! not valid Xij numerical value!" << endl;
            delete given_Xs;
            delete item_id;
            return nullptr;
        }

        given_Xs->push_back(Xnum);
    }

    TimeSeries *p = new TimeSeries(given_Xs,item_id);

    return p;
}

int FileReader::readConfigFile(int &K, int &L, int &k_lsh, int &M, int &k_hc, int &probes){
    K=-1; L=-1; k_lsh=-1; M=-1; k_hc=-1; probes=-1;
    if( configuration_file.is_open() == false ) return -1;

    string line, argument_name,argument_value;
    int *argument;

    while (getline(configuration_file, line)){
        istringstream line_stream(line);

        line_stream >> argument_name;

        if(argument_name.compare("number_of_clusters:") == 0){
            argument = &K;
        }else if(argument_name.compare("number_of_vector_hash_tables:") == 0){
            argument = &L;
        }else if(argument_name.compare("number_of_vector_hash_functions:") == 0){
            argument = &k_lsh;
        }else if(argument_name.compare("max_number_M_hypercube:") == 0){
            argument = &M;
        }else if(argument_name.compare("number_of_hypercube:") == 0){
            argument = &k_hc;
        }else if(argument_name.compare("number_of_probes:") == 0){
            argument = &probes;
        }

        line_stream >> argument_value;
        try {
            *argument = stoi(argument_value);
        }catch(...) {
            cout << "Error: invalid configuration file format!" << endl;
            return -2;
        }

    }

    if(K == -1){
        cout << "cube: No number_of_clusters parameter found in the configuration file!" << endl;
        return -1;
    }

    bool used_default = false;
    if( L==-1 || k_lsh==-1 || M==-1 || k_hc==-1 || probes==-1 ){
        cout << "Using default values for:";
        used_default = true;
    }

    if(L == -1){
        L = 3;
        cout << " number_of_vector_hash_tables,";
    }
    if(k_lsh == -1){
        k_lsh = 4;
        cout << " number_of_vector_hash_functions,";
    }
    if(M == -1){
        M = 10;
        cout << " max_number_M_hypercube,";
    }
    if(k_hc == -1){
        k_hc = 3;
        cout << " number_of_hypercube,";
    }
    if(probes == -1){
        probes =2;
        cout << " number_of_probes,";
    }

    if(used_default) cout << endl;

    return 0;
}

TimeSeries* FileReader::getQuery(string id){
    auto p = queries.find(id);
    if( p == queries.end() ) return nullptr;
    return p->second;
}

int FileReader::writeQuery(const string& query_id, PD *knn, PD* bruteForce, int k, char mode, double &MAF, char fr_mode){
    char const *method = "Unknown";
    switch (mode){
        case __LSH_MODE:
            method = "LSH_Vector";
            break;
        case __H_CUBE_MODE:
            method = "Hypercube";
            break;
        case __FRECHET_DISCRETE_MODE:
            method = "LSH_Frechet_Discrete";
            break;
        case __FRECHET_CONTINUOUS_MODE:
            method = "LSH_Frechet_Continuous";
            break;
    }

    output_file << "Query: " << query_id << "\nAlgorithm: " << method << '\n';
    for(int i=0; i<k; i++){
        if( knn[i].p != nullptr ){
            output_file << "Aproximate Nearest neighbor: " << knn[i].p->getId() << '\n';
        }else output_file << "Aproximate Nearest neighbor: -\n";
        if(bruteForce!=nullptr){
            output_file  << "True Nearest neighbor: " << bruteForce[i].p->getId() << '\n';
        }
        if( knn[i].p != nullptr ){
            output_file << "distanceAproximate: " << knn[i].distance << '\n';
        }else output_file << "distanceAproximate: -\n";
        if(bruteForce!=nullptr){
            output_file << "distanceTrue: " << bruteForce[i].distance << "\n";
        }
        output_file << '\n';
        if(bruteForce!=nullptr)
            MAF = max( MAF, knn[i].distance / bruteForce[i].distance );
    }

    return 0;
}

int FileReader::writeQueryTimes(double timeAprx, double timeBF, int count, double MAF, bool bruteForce){
    output_file << "tAproximateAverage: " << timeAprx/count << "s\n";
    if(bruteForce){
        output_file << "tTrueAverage: " << timeBF/count << "s\n"
                    << "MAF: " << MAF << '\n' << endl;
    }

    return 0;
}

int FileReader::writeRangeNeighbors(std::unordered_map<std::string, TimeSeries*> neighbors){
    output_file << "R-near neighbors:" << '\n';
    for(auto neighbor: neighbors){
        output_file << neighbor.second->getId() << '\n';
    }
    output_file << endl;
    return 0;
}

int FileReader::writeClusterPoints(std::unordered_map<std::string, TimeSeries*> *Clusters, int clustering_times, ClusterObject *Medoids, int k, const char* algorithm, bool complete){
    output_file << "Algorithm: " << algorithm << '\n';
    for(int i=0; i<k; i++){
        output_file << "CLUSTER-" << i << " {size: " << Clusters[i].size() << " centroid:";
        for(auto X: Medoids[i]->getXs())
            output_file << ' ' << X;
        output_file << "}\n";
    }

    if(complete){
        for(int i=0; i<k; i++){
            output_file << "CLUSTER-" << i << " {";
            for(auto X: Medoids[i]->getXs())
                output_file << ' ' << X;
            output_file <<  " ,";

            for(auto pointPair: Clusters[i]){
                for(auto X: pointPair.second->getXs())
                    output_file << ' ' << X;
                output_file << ", ";
            }
            output_file << "}\n";
        }
    }

    output_file << "clustering_time: " << millisecToSec(clustering_times) << endl;
    return 0;
}

int FileReader::writeSilhouette(silhouetteStats *silhouetteS, int k){
    output_file << "Silhouette: [";
    for(int i=0; i<k; i++)
        output_file << silhouetteS->avgSil[i] << ", ";
    output_file << silhouetteS->OSC << "]" << endl;
    return 0;
}

FileReader::~FileReader(){
    input_file.close();
    if( query_file.is_open() ) query_file.close();
    output_file.close();
    if( configuration_file.is_open() ) configuration_file.close();

    for(auto it: queries)
        delete it.second;
}