#include "Point.h"

Point::Point(std::vector<int> *given_Xs, std::string *given_item_id)
: X(given_Xs), item_id(given_item_id){}

Point::~Point(){
    delete X;
    delete item_id;
}