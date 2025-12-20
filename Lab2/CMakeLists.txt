cmake_minimum_required(VERSION 3.14)
project(example)

set(CMAKE_CXX_STANDARD 20)

# Download and build GoogleTest using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows, prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(example
        main.cpp
        Array.h
)

add_executable(tests
        tests.cpp
        Array.h
)

# Link with GTest::gtest_main instead of gtest
target_link_libraries(tests GTest::gtest_main)