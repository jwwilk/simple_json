# Distributed under the MIT License, see accompanying file LICENSE.txt
# Copyright John W. Wilkinson 2025

set(GTEST_SOURCE_DIR "C:/Users/johnw/Documents/git/googletest-1.17.0")
set(GTEST_ROOT "${GTEST_SOURCE_DIR}_build")
include_directories("${GTEST_SOURCE_DIR}/googletest/include")

# Add source to this project's executable.
add_executable (test "simple_json_test.cpp")

target_link_libraries( test PRIVATE simple_json
    "$<$<CONFIG:Debug>:${GTEST_ROOT}/lib/Debug/gtest.lib>"
    "$<$<CONFIG:Debug>:${GTEST_ROOT}/lib/Debug/gtest_main.lib>"
    "$<$<CONFIG:Release>:${GTEST_ROOT}/lib/Release/gtest.lib>"
    "$<$<CONFIG:Release>:${GTEST_ROOT}/lib/Release/gtest_main.lib>"
    "$<$<CONFIG:RelWithDebInfo>:${GTEST_ROOT}/lib/RelWithDebInfo/gtest.lib>"
    "$<$<CONFIG:RelWithDebInfo>:${GTEST_ROOT}/lib/RelWithDebInfo/gtest_main.lib>"
)

include(GoogleTest)
gtest_discover_tests(test)
