#pragma once

#include <vector>
#include <string>

class Point{
    private:
        std::vector<int> *X;
        std::string *item_id;
    public:
        Point(std::vector<int> *given_Xs, std::string *given_item_id);
        Point(Point &p);
        inline /*const*/ std::string getId() { return *item_id; };
        inline const std::vector<int>& getXs() { return *X; };
        void print();
        ~Point();
};


