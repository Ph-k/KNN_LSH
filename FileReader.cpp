#include "FileReader.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
#include <stdio.h>
FileReader::FileReader(char const *input_f, char const *query_f, char const *output_f)
:dimension(-1){
    input_file.open(input_f);
    query_file.open(query_f);
    output_file.open(query_f);
};

Point* FileReader::ReadPoint(){
    //vector<int> *v = new vector<int>;

    string line;
    if ( input_file.peek() != EOF ) 
        getline(input_file, line);
    else
        return nullptr;

    istringstream line_stream(line);

    vector<int> *given_Xs = new vector<int>;

    int s=0,Xnum;
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
        s++;
    }

    if(dimension==-1) dimension = s - 1;

    Point *p = new Point(given_Xs,item_id);

    return p;
}


FileReader::~FileReader(){
    input_file.close();
    input_file.close();
    input_file.close();
}