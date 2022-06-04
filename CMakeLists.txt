cmake_minimum_required(VERSION 3.22)
project(diploma)

set(CMAKE_CXX_STANDARD 20)

add_executable(diploma main.cpp Tree.h DataRestriction.h Data.h DataReader.h Utils.h TextFileReader.h DataProcessor.h)
