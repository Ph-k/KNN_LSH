#include <iostream>
#include "Point.h"

using namespace std;

Point::Point(std::vector<int> *given_Xs, std::string *given_item_id)
: X(given_Xs), item_id(given_item_id){}


void Point::print(){
    cout << '{';
    for(auto it: *X)
        cout << it << ", ";
    cout << '}' << endl;
}


Point::~Point(){
    delete X;
    delete item_id;
}