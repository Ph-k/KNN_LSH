#pragma once

#include <vector>
#include <string>

class TimeSeries{
    private:
        std::vector<int> *X;
        std::string *item_id;
    public:
        TimeSeries(std::vector<int> *given_Xs, std::string *given_item_id);
        TimeSeries(TimeSeries &p);
        inline /*const*/ std::string getId() { return *item_id; };
        inline const std::vector<int>& getXs() { return *X; };
        void print();
        ~TimeSeries();
};


