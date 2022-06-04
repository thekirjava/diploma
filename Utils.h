#pragma once

#include <tuple>
#include <iostream>

using namespace std;

template<bool flag, int lhs, int rhs>
struct Min {
    enum {
        minValue = lhs
    };
};

template<int lhs, int rhs>
struct Min<false, lhs, rhs> {
    enum {
        minValue = rhs
    };
};

template<typename Tuple1, typename Tuple2, int N>
struct ComparatorHelper {
    static int compare(const Tuple1 &lhs, const Tuple2 &rhs) {
        if constexpr(N != 1) {
            return middleCompare(lhs, rhs);
        } else {
            return firstCompare(lhs, rhs);
        }
    }

    static int nodeCompare(const Tuple1 &lhs, const Tuple2 &rhs, int &levelDiff) {
        if constexpr(N != 1) {
            return middleNodeCompare(lhs, rhs, levelDiff);
        } else {
            return firstNodeCompare(lhs, rhs, levelDiff);

        }
    }

private:
    static int middleCompare(const Tuple1 &lhs, const Tuple2 &rhs) {
        int result = ComparatorHelper<Tuple1, Tuple2, N - 1>::compare(lhs, rhs);
        if (result == 0) {
            if (get<N - 1>(lhs) > get<N - 1>(rhs)) {
                result = 1;
            } else if (get<N - 1>(lhs) < get<N - 1>(rhs)) {
                result = -1;
            } else {
                result = 0;
            }
        }
        return result;
    }

    static int firstCompare(const Tuple1 &lhs, const Tuple2 &rhs) {
        if (get<0>(lhs) > get<0>(rhs)) {
            return 1;
        } else if (get<0>(lhs) < get<0>(rhs)) {
            return -1;
        } else {
            return 0;
        }
    }

    static int middleNodeCompare(const Tuple1 &lhs, const Tuple2 &rhs, int &levelDiff) {
        int isCompareNext = ComparatorHelper<Tuple1, Tuple2, N - 1>::nodeCompare(lhs, rhs, levelDiff);
        if (isCompareNext) {
            if (get<N - 1>(lhs) == get<N - 1>(rhs)) {
                levelDiff++;
                return true;
            }
        }
        return false;
    }

    static int firstNodeCompare(const Tuple1 &lhs, const Tuple2 &rhs, int &levelDiff) {
        if (get<0>(lhs) == get<0>(rhs)) {
            levelDiff++;
            return true;
        }
        return false;
    }
};

template<typename... Args1, typename... Args2>
int compare(const tuple<Args1...> &lhs, const tuple<Args2...> &rhs) {
    return ComparatorHelper<
            decltype(lhs), decltype(rhs),
            Min<sizeof...(Args1) < sizeof...(Args2), sizeof...(Args1), sizeof...(Args2)>::minValue>::compare(lhs, rhs);
}

template<typename... Args1, typename... Args2>
int nodeCompare(const tuple<Args1...> &t1, const tuple<Args2...> &t2, int &levelDiff) {
    return ComparatorHelper<decltype(t1), decltype(t2), Min<
            sizeof...(Args1) < sizeof...(Args2), sizeof...(Args1), sizeof...(Args2)>::minValue>::nodeCompare(t1, t2,
                                                                                                             levelDiff);
}

// ===============================================================
template<class Tuple, int N>
struct TuplePrinter {
    static ostream &write(ostream &out, const Tuple &tuple) {
        TuplePrinter<Tuple, N - 1>::write(out, tuple);
        out << " " << get<N - 1>(tuple);
        return out;
    }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
    static ostream &write(ostream &out, const Tuple &tuple) {
        out << get<0>(tuple);
        return out;
    }
};

template<class CharT, class Traits, class... Types>
std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const tuple<Types...> &outputTuple) {
    return TuplePrinter<decltype(outputTuple), tuple_size<tuple<Types...>>::value>::write(out, outputTuple);
}

template<class CharT, class Traits>
std::basic_istream<CharT, Traits> &operator>>(std::basic_istream<CharT, Traits> &in, std::tuple<> &inputTuple);

template<class CharT, class Traits, class FirstType, class... OtherTypes>
std::basic_istream<CharT, Traits> &
operator>>(std::basic_istream<CharT, Traits> &in, tuple<FirstType, OtherTypes...> &inputTuple) {
    FirstType inputValue;
    tuple<OtherTypes...> otherInput;

    in >> inputValue;
    in >> otherInput;

    inputTuple = tuple_cat(make_tuple(inputValue), otherInput);
    return in;
}

template<class CharT, class Traits>
std::basic_istream<CharT, Traits> &operator>>(std::basic_istream<CharT, Traits> &in, std::tuple<> &inputTuple) {
    return in;
}

// =========================================================================================

template<int n, typename ConditionalFunctionObject>
struct ValueCondition {
    explicit ValueCondition(ConditionalFunctionObject condition) : condition(condition) {}

    enum {
        valueNumber = n
    };
    ConditionalFunctionObject condition;
};

template<int N, typename Tuple, typename TupleCondition>
struct DataConditionEvaluate {
    static void isCondition(bool &isCond, Tuple &t, TupleCondition &tc) {
        DataConditionEvaluate<N - 1, Tuple, TupleCondition>::isCondition(isCond, t, tc);
        auto valueCondition = get<N - 1>(tc);
        auto value = get<decltype(valueCondition)::valueNumber>(t);

        isCond &= valueCondition.condition(value);
    }
};

template<typename Tuple, typename TupleCondition>
struct DataConditionEvaluate<1, Tuple, TupleCondition> {
    static void isCondition(bool &isCond, Tuple &t, TupleCondition &tc) {
        auto valueCondition = get<0>(tc);
        auto value = get<decltype(valueCondition)::valueNumber>(t);
        isCond &= valueCondition.condition(value);
    }
};

template<int... n, typename... ConditionalFunctionObject, typename... Types>
bool isCondition(tuple<ValueCondition<n, ConditionalFunctionObject>...> &valueConditions, tuple<Types...> &values) {
    bool isCond = true;
    DataConditionEvaluate<sizeof...(n), decltype(values), decltype(valueConditions)>::isCondition(isCond,
                                                                                                  values,
                                                                                                  valueConditions);
    return isCond;
}

template<typename... Types>
struct DataCondition;

template<int m, int... n, typename ConditionalFunctionObject, typename... otherConditionalFunctionObject>
struct DataCondition<ValueCondition<m, ConditionalFunctionObject>, ValueCondition<n, otherConditionalFunctionObject>...>
        : DataCondition<ValueCondition<n, otherConditionalFunctionObject>...> {
    explicit DataCondition(ValueCondition<m, ConditionalFunctionObject> &valueCondition,
                           ValueCondition<n, otherConditionalFunctionObject> &... otherConditions) :
            valueCondition(valueCondition),
            DataCondition<ValueCondition<n, otherConditionalFunctionObject>...>(otherConditions...) {}

    ValueCondition<m, ConditionalFunctionObject> valueCondition;
};

template<int n, typename ConditionalFunctionObject>
struct DataCondition<ValueCondition<n, ConditionalFunctionObject>> {
    explicit DataCondition(ValueCondition<n, ConditionalFunctionObject> &valueCondition) : valueCondition(
            valueCondition) {}

    ValueCondition<n, ConditionalFunctionObject> valueCondition;
};

template<int n, typename ConditionalFunctionObject>
auto makeValueCondition(ConditionalFunctionObject condition) {
    return ValueCondition<n, ConditionalFunctionObject>(condition);
}

template<int... n, typename ConditionalFunctionObject>
auto makeDataCondition(ValueCondition<n, ConditionalFunctionObject>... valueConditions) {
    return DataCondition<ValueCondition<n, ConditionalFunctionObject>...>(valueConditions...);
}
