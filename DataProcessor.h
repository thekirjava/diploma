#pragma once

#include <iostream>
#include <utility>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

template<typename ResultType, typename TreeIterator, typename Operation, typename ValueExtractor>
void processTree(TreeIterator first, TreeIterator second, Operation operation, ValueExtractor extractor,
                 ostream &out = cout) {
    vector<ResultType> result(first.getKeyCount());
    for (auto it = first; it != second; it++) {
        int currentDepth = it.getDepth();
        if (it.isLeaf()) {
            ResultType currentValue = extractor(*it);
            result[currentDepth - 1] = operation(result[currentDepth - 1], currentValue);
            // вывод листа
            out << (*it).keyTuple() << ": " << currentValue << '\n';
        } else {
            // вывод result[currentDepth]
            auto keyTuple = (*it).keyTuple();
            stringstream ss;
            ss << keyTuple;
            string strTuple = ss.str();
            for (int i = currentDepth; i < tuple_size<decltype(keyTuple)>(); i++) {
                strTuple.pop_back();
                if (!strTuple.empty()) {
                    strTuple.pop_back();
                }
            }
            if (strTuple.empty()) {
                strTuple = "tree";
            }
            out << strTuple << ": " << result[currentDepth] << '\n';
            if (currentDepth > 0) {
                result[currentDepth - 1] = operation(result[currentDepth - 1], result[currentDepth]);
            }
            result[currentDepth] = {};
        }
    }
}

template<typename ResultType, typename TreeIterator, typename ValueExtractor>
void sumTree(TreeIterator first, TreeIterator second,  ValueExtractor extractor,
             ostream &out=cout) {
    processTree<ResultType>(first, second,  [](const ResultType &lhs, const ResultType &rhs) {
        return lhs + rhs;
    }, extractor, out);
}

template<typename ResultType, typename TreeIterator, typename ValueExtractor>
void mulTree(TreeIterator first, TreeIterator second,  ValueExtractor extractor,
             ostream &out=cout) {
    processTree<ResultType>(first, second,  [](const ResultType &lhs, const ResultType &rhs) {
        return lhs * rhs;
    }, extractor, out);
}

template<typename ResultType, typename TreeIterator, typename ValueExtractor>
void maxTree(TreeIterator first, TreeIterator second,  ValueExtractor extractor,
             ostream &out=cout) {
    processTree<ResultType>(first, second,  [](const ResultType &lhs, const ResultType &rhs) {
        return max(lhs, rhs);
    }, extractor, out);
}

template<typename ResultType, typename TreeIterator, typename ValueExtractor>
void minTree(TreeIterator first, TreeIterator second,  ValueExtractor extractor,
             ostream &out = cout) {
    processTree<ResultType>(first, second,  [](const ResultType &lhs, const ResultType &rhs) {
        return min(lhs, rhs);
    }, extractor, out);
}

template<typename ResultType, typename TreeIterator, typename ValueExtractor>
void gcdTree(TreeIterator first, TreeIterator second,  ValueExtractor extractor,
             ostream &out=cout) {
    processTree<ResultType>(first, second,  [](const ResultType &lhs, const ResultType &rhs) {
        return gcd(lhs, rhs);
    }, extractor, out);
}

template<typename ResultType, typename TreeIterator, typename ValueExtractor>
void lcmTree(TreeIterator first, TreeIterator second,  ValueExtractor extractor,
             ostream &out=cout) {
    processTree<ResultType>(first, second,  [](const ResultType &lhs, const ResultType &rhs) {
        return lcm(lhs, rhs);
    }, extractor, out);
}