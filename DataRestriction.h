#pragma once

#include <tuple>
#include "Data.h"
#include "Utils.h"

using namespace std;

template<typename Data, typename Conditions>
class DataRestriction;

template<typename... Types, int... KeyIdxs, int... n, typename... ConditionalFunctionObject>
class DataRestriction<Data<tuple<Types...>, key<KeyIdxs...>>, tuple<ValueCondition<n, ConditionalFunctionObject>...>> {
public:
    typedef Data<tuple<Types...>, key<KeyIdxs...>> DataType;
    typedef tuple<ValueCondition<n, ConditionalFunctionObject>...> ValueConditionType;

    DataRestriction() = default;

    explicit DataRestriction(ValueConditionType
                           conditions) :
            conditions(conditions) {}

    bool check(DataType *data) {
        return isCondition(conditions, data->values);
    }

private:
    ValueConditionType conditions;
};

class noRestriction {
public:
    template<typename Data>
    bool check(Data data) {
        return true;
    }
};