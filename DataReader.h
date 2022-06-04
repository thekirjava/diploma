#pragma once

#include <tuple>
#include "DataRestriction.h"
#include "Data.h"

using namespace std;

template<typename Data, typename DataRestriction = noRestriction>
class DataReader;

template<typename DataRestriction, typename... TupleTypes, int... Idxs>
class DataReader<Data<tuple<TupleTypes...>, key<Idxs...>>, DataRestriction> {
    typedef tuple<TupleTypes...> Tuple;
    typedef key<Idxs...> KeyIndexes;
    typedef Data<Tuple, KeyIndexes> DataType;
public:
    DataRestriction restriction = DataRestriction();

    DataReader() {};

    DataReader(DataRestriction &dataRestriction) : restriction(dataRestriction) {}

    virtual DataType *getCurrent() = 0;

    virtual DataType *getNext() = 0;

    virtual bool hasNext() = 0;

    virtual DataType *read() = 0;

    virtual DataReader<DataType, DataRestriction>* makeCopy() = 0;
};