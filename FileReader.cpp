#include "FileReader.h"
#include "Utilities.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

FileReader::FileReader(char const *input_f, char const *query_f, char const *output_f)
{
    input_file.open(input_f);
    query_file.open(query_f);
    output_file.open(output_f);

    dimension = this->find_dimension_from_input(input_f);
    if(dimension == -1) cout << "Error: invalid file format!" << endl;


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
                        << "tTrue:" << timeBF << "ms" << '\n'; 
        }
    }
    output_file << "R-near neighbors:" << endl;
    return 0;
}

int FileReader::writeRangeNeighbor(const string& neighbor_id){
    output_file << neighbor_id << '\n';
    return 0;
}

FileReader::~FileReader(){
    input_file.close();
    query_file.close();
    output_file.close();

    for(auto it: queries)
        delete it.second;
}