#include <iostream>
#include "Point.h"

using namespace std;

Point::Point(std::vector<int> *given_Xs, std::string *given_item_id)
: X(given_Xs), item_id(given_item_id){}

Point::Point(Point &p){
    this->X = new vector<int>(p.getXs());
    this->item_id = new string(p.getId());
}

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