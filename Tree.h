#pragma once

#include "DataRestriction.h"
#include "Data.h"
#include "tuple"

using namespace std;

template<typename DataReader, typename DataRestriction = noRestriction>
class Tree;

template<template<class, class> class DataReader, typename DataReaderRestriction, typename DataRestriction, typename... Types, int... KeyIdxs>
class Tree<DataReader<Data<tuple<Types...>, key<KeyIdxs...>>, DataReaderRestriction>, DataRestriction> {
    typedef std::tuple<Types...> Tuple;
    typedef key<KeyIdxs...> KeyIndexes;
    typedef Data<Tuple, KeyIndexes> DataType;
    typedef DataReader<DataType, DataReaderRestriction> Data;
    typedef DataRestriction RestrictionType;


    class TreeIterator {
    public:
        explicit TreeIterator(Data *data, RestrictionType &restriction) : data(data), restriction(restriction) {}

        DataType operator*() {
            return *data->getCurrent();
        }

        DataType *operator->() {
            return data->getCurrent();
        }

        TreeIterator operator++() {
            next();
            return *this;
        }

        bool operator!=(const TreeIterator &other) {
            if (data == nullptr && (other.data == nullptr || !other.data->hasNext()) && other.depth == -1) {
                return false;
            }
            if ((data == nullptr || !data->hasNext()) && depth == -1 && other.data == nullptr) {
                return false;
            }
            if (data == nullptr || other.data == nullptr) {
                return this->data != other.data;
            }
            return (data->getCurrent() != other.data->getCurrent()) || (depth != other.depth);
        }

        TreeIterator operator++(int) {
            TreeIterator iter = *this;
            next();
            return iter;
        }

        bool isRestrictionChecked() {
            return restriction.check(this->data->getCurrent());
        }

        bool hasNext() {
            return data != nullptr && (depth != 0 || data->getNext() != nullptr);
        }

        bool isLeaf() {
            return depth == keyCount;
        }

        int getDepth() {
            return depth;
        }

        template<int N>
        auto getValue() {
            return get<N>(data->getCurrent()->values);
        }

        int getKeyCount() const {
            return keyCount;
        }

    private:
        void next() {
            if (depth >= 0) {
                depth--;
                if (data->hasNext()) {
                    auto key = data->getCurrent()->keyTuple();
                    auto nextKey = data->getNext()->keyTuple();
                    int diff = 0;
                    nodeCompare(key, nextKey, diff);
                    if (diff == depth) {
                        data->read();
                        depth = keyCount;
                    }
                }
            }
        }

        Data *data;
        RestrictionType &restriction;
        int keyCount = sizeof...(KeyIdxs);
        int depth = keyCount;
    };

public:
    explicit Tree(Data &data) : data(data) {}

    explicit Tree(Data &data, DataRestriction &dataRestriction) : data(data), restriction(dataRestriction) {}

    TreeIterator begin() {
        return TreeIterator(data.makeCopy(), restriction);
    }

    TreeIterator end() {
        return TreeIterator(nullptr, restriction);
    }

private:
    Data &data;
    DataRestriction restriction = RestrictionType();

};

