#pragma once

#include <tuple>
#include "Utils.h"

using namespace std;

template<typename Tuple, typename Key>
class Data;

template<typename Keys>
class Key;

template<int... Nums>
struct key {

};

template<int... Indexes, typename Tuple>
decltype(auto) getKeys(Tuple &&t) {
    return make_tuple(get<Indexes>(forward<Tuple>(t))...);
}

template<typename... Types>
class Key<tuple<Types...>> {
public:
    typedef tuple<Types...> Tuple;
    Tuple keys;

    explicit Key(const Tuple &keys) : keys(keys) {};

    template<typename KeyType>
    bool operator<(const KeyType &other) {
        return compare(keys, other.keys) < 0;
    }

    template<typename KeyType>
    bool operator>(const KeyType &other) {
        return compare(keys, other.keys) > 0;
    }

    template<typename KeyType>
    bool operator==(const KeyType &other) {
        return compare(keys, other.keys) == 0;
    }
};

template<typename... Types, int... Nums>
class Data<tuple<Types...>, key<Nums...>> {
public:
    typedef tuple<Types...> Tuple;
    Tuple values;

    Data() = default;

    explicit Data(Tuple values) : values(values) {}

    friend ostream &operator<<(ostream &os, const Data &data) {
        os << data.values;
        return os;
    }

    auto key() const {
        auto keyValues = getKeys<Nums...>(values);
        return Key<decltype(keyValues)>(keyValues);
    }

    auto keyTuple() const {
        return getKeys<Nums...>(values);
    }

    ~Data() = default;
};