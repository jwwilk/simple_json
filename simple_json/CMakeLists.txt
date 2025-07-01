# Distributed under the MIT License, see accompanying file LICENSE.txt
# Copyright John W. Wilkinson 2025

SET(SRCS simple_json.cpp simple_json.h)
ADD_LIBRARY(simple_json STATIC ${SRCS})
target_include_directories(simple_json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})