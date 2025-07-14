# Distributed under the MIT License, see accompanying file LICENSE.txt
# Copyright John W. Wilkinson 2025

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add source to this project's executable.
add_executable (simple_json_test "simple_json_test.cpp")

# Link test with main library and GoogleTest
target_link_libraries(simple_json_test
    PRIVATE
    simple_json
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(simple_json_test)
