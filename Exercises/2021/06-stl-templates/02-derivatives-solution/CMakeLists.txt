cmake_minimum_required(VERSION 3.0)

project(Derivatives VERSION 1.0 DESCRIPTION "a very nice project"
        LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++17")
add_executable(test-exec main.cpp)
