# Distributed under the MIT License, see accompanying file LICENSE.txt
# Copyright John W. Wilkinson 2025

add_library(simple_json STATIC simple_json.cpp)
target_sources(simple_json PRIVATE simple_json.h)
target_include_directories(simple_json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})