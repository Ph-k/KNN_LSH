#include "FileReader.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

FileReader::FileReader(char const *input_f, char const *query_f, char const *output_f)
{
    input_file.open(input_f);
    query_file.open(query_f);
    output_file.open(query_f);

    dimension = this->find_dimension_from_input(input_f);
    if(dimension == -1) cout << "Error: invalid file format!" << endl;
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

Point* FileReader::ReadPoint(){
    //vector<int> *v = new vector<int>;

    string line;
    if ( input_file.peek() != EOF ) 
        getline(input_file, line);
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


FileReader::~FileReader(){
    input_file.close();
    input_file.close();
    input_file.close();
}