#pragma once

#include "Utils.h"
#include "DataReader.h"
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template<typename Data, typename DataRestriction = noRestriction>
class TextFileReader;

template<typename DataRestriction, typename... Types, int... Keys>
class TextFileReader<Data<tuple<Types...>, key<Keys...>>, DataRestriction>
        : public DataReader<Data<tuple<Types...>, key<Keys...>>, DataRestriction> {
public:
    typedef Data<tuple<Types...>, key<Keys...>> DataType;
    typedef DataReader<Data<tuple<Types...>, key<Keys...>>, DataRestriction> DataSource;

    explicit TextFileReader(const string &fileName) : fileName(fileName) {
        in.open(fileName);
        read();
        read();
    }

    explicit TextFileReader(const string &fileName, DataRestriction &restriction) : DataSource(restriction),
                                                                                  fileName(fileName) {
        in.open(fileName);
        read();
        read();
    }

    TextFileReader(const TextFileReader &other) : TextFileReader(other.fileName) {}

    DataType *getCurrent() override {
        if (current == nullptr) {
            read();
        }
        return current;
    }

    DataType *getNext() override {
        return next;
    }

    DataType *read() override {
        tuple<Types...> inputTuple;
        current = next;
        if (!isHasNext) {
            return nullptr;
        }
        bool readFlag, restrictionFlag;
        do {
            if (in >> inputTuple) {
                readFlag = true;
                next = new DataType(inputTuple);
                restrictionFlag = !this->restriction.check(next);
            } else {
                readFlag = false;
                isHasNext = false;
                next = nullptr;
            }
        } while (readFlag && restrictionFlag);
        return current;
    }

    bool hasNext() override {
        return isHasNext;
    }

    TextFileReader<DataType, DataRestriction> *makeCopy() override {
        return new TextFileReader<DataType, DataRestriction>(fileName, this->restriction);
    }

    virtual ~TextFileReader() {
        in.close();
    }

private:
    ifstream in;
    string fileName;
    bool isHasNext = true;
    DataType *current = nullptr;
    DataType *next = nullptr;
};