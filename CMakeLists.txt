# Distributed under the MIT License, see accompanying file LICENSE.txt
# Copyright John W. Wilkinson 2025

cmake_minimum_required (VERSION 4.0.0)

project ("simple_json")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # enforce the standard (don’t allow fallback)
set(CMAKE_CXX_EXTENSIONS OFF)         # disable compiler-specific extensions (like GNU++17)

add_subdirectory ("simple_json")
add_subdirectory ("test")
