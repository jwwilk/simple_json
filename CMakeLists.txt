# Distributed under the MIT License, see accompanying file LICENSE.txt
# Copyright John W. Wilkinson 2025

cmake_minimum_required (VERSION 4.0.0)

project ("simple_json")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory ("simple_json")

option(SIMPLE_JSON_BUILD_TESTS "Build the Simple JSON test executable" ON)
if(SIMPLE_JSON_BUILD_TESTS)
    add_subdirectory ("simple_json_test")
endif()
