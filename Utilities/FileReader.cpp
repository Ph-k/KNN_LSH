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

    query_file.open(query_f);
    if( query_file.is_open() == false )
        {cout << "ERROR: could not open query file: " << query_f << endl; exit(-1);}

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


    Point *p = ReadPoint('q');
    while( p != nullptr ){
        pair<string, Point*> temp(p->getId(),p);
        queries.insert( temp );
        p = ReadPoint('q');
    }

    cout << "A file manager created! " << endl;
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

Point* FileReader::ReadPoint(char file_mode){
    //vector<int> *v = new vector<int>;

    ifstream *file = nullptr;
    switch (file_mode){
        case 'i':
            file = &(this->input_file);
            break;
        case 'q':
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

    vector<int> *given_Xs = new vector<int>;

    int Xnum;
    string Xstring, *item_id = new string;
    line_stream >> *item_id;
    while (line_stream >> Xstring){

        try {
            Xnum = stoi(Xstring);
        }catch(...) {
            cout << "Error: invalid input file format! not valid Xij numerical value!" << endl;
            delete given_Xs;
            delete item_id;
            return nullptr;
        }

        given_Xs->push_back(Xnum);
    }

    Point *p = new Point(given_Xs,item_id);

    return p;
}

int FileReader::readConfigFile(int &K, int &L, int &k_lsh, int &M, int &k_hc, int &probes){
    K=-1; L=3; k_lsh=4; M=10; k_hc=3; probes=2;
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
        }else if(argument_name.compare("mac_number_M_hypercube:") == 0){
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

    return 0;
}

Point* FileReader::getQuery(string id){
    return queries.find(id)->second;
}

int FileReader::writeLshQuery(const string& query_id, PD *knn, PD* bruteForce, int k, double timeLSH, double timeBF){
    output_file << "Query: " << query_id << '\n';
    for(int i=0; i<k; i++){
        if( knn[i].p != nullptr ){
            output_file << "Nearest neighbor-" << i+1 << ": " << knn[i].p->getId() << '\n'
                        << "distanceLSH: " << knn[i].distance << '\n'
                        << "distanceTrue: " << bruteForce[i].distance << '\n'
                        << "tLSH:" << timeLSH << "ms" << '\n'
                        << "tTrue:" << timeBF << "ms" << '\n'; 
        }else{
            output_file << "Nearest neighbor-" << i+1 << ": none\n"
                        << "distanceLSH: -\n"
                        << "distanceTrue: " << bruteForce[i].distance << '\n'
                        << "tLSH:" << timeLSH << "ms" << '\n'
                        << "tTrue:" << timeBF << "ms" << endl; 
        }
    }

    return 0;
}

int FileReader::writeRangeNeighbors(std::unordered_map<std::string, Point*> neighbors){
    output_file << "R-near neighbors:" << '\n';
    for(auto neighbor: neighbors){
        output_file << neighbor.second->getId() << '\n';
    }
    output_file << endl;
    return 0;
}

FileReader::~FileReader(){
    input_file.close();
    query_file.close();
    output_file.close();
    if( configuration_file.is_open() ) configuration_file.close();

    for(auto it: queries)
        delete it.second;
}