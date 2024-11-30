cmake_minimum_required(VERSION 3.28)
project(Discrete_Maths)

set(CMAKE_CXX_STANDARD 17)

add_executable(Discrete_Maths main.cpp
        LogicalExpressionEvaluator.h
        TruthTable.h)
