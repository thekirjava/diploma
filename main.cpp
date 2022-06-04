#include <iostream>
#include "Utils.h"
#include "TextFileReader.h"
#include "Data.h"
#include <string>
#include <utility>
#include "Tree.h"
#include <vector>
#include "DataProcessor.h"

using namespace std;

struct StudentInfo {
    string name;
    float gpa;

    friend ostream &operator<<(ostream &out, const StudentInfo &value) {
        out << "Student " << value.name << ": " << value.gpa;
        return out;
    }

    bool operator<(const StudentInfo &other) const {
        return this->gpa < other.gpa;
    }
};

void maxTree(const string &inputFileName, const string &outputFileName) {
    typedef tuple<string, int, int, int, int, float> tupleType;
    typedef key<1, 2, 3, 4> keyType;
    typedef Data<tupleType, keyType> dataType;

    auto value1Restriction = [](int facultyNumber) { return facultyNumber >= 1; };
    auto value2Restriction = [](int courseNumber) { return courseNumber >= 1; };
    auto value3Restriction = [](int groupNumber) { return groupNumber >= 1; };
    auto value4Restriction = [](int studentNumber) { return studentNumber >= 1; };
    auto value5Restriction = [](float gpa) { return (gpa >= 4.0) && (gpa <= 10.0); };

    auto restrictionTuple = make_tuple(makeValueCondition<1>(value1Restriction),
                                       makeValueCondition<2>(value2Restriction),
                                       makeValueCondition<3>(value3Restriction),
                                       makeValueCondition<4>(value4Restriction),
                                       makeValueCondition<5>(value5Restriction));
    DataRestriction<dataType, decltype(restrictionTuple)> restriction(restrictionTuple);
    TextFileReader<dataType, decltype(restriction)> fileReader(inputFileName, restriction);
    Tree<TextFileReader<dataType, decltype(restriction)>> tree(fileReader);
    ofstream fout(outputFileName);
    auto studentInfoComparator = [](const StudentInfo &lhs, const StudentInfo &rhs) { return lhs.gpa < rhs.gpa; };
    maxTree <StudentInfo> (tree.begin(), tree.end(), [](dataType data) {
        return StudentInfo {get<0>(data.values), get<5>(data.values)};
    }, fout);
}

int main() {
    maxTree("input.txt", "output.txt");
    maxTree("incorrect_input.txt", "filtered_output.txt");
}
