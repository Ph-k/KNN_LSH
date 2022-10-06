#pragma once

#include <vector>
#include <string>

#define __TIMESERIES_X_TYPE double

class TimeSeries{
    private:
        std::vector<__TIMESERIES_X_TYPE> *X;
        std::string *item_id;
    public:
        TimeSeries(std::vector<__TIMESERIES_X_TYPE> *given_Xs, std::string *given_item_id);
        TimeSeries(TimeSeries &p);
        inline /*const*/ std::string getId() { return *item_id; };
        inline const std::vector<__TIMESERIES_X_TYPE>& getXs() { return *X; };
        void print();
        ~TimeSeries();
};


