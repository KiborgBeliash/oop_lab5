cmake_minimum_required(VERSION 3.15)
project(MemoryPoolProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable
add_executable(memory
    src/main.cpp
    src/memory_pool.cpp
)

target_include_directories(memory PRIVATE include)

# GoogleTest через FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# Для Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Tests
add_executable(tests
    tests/tests.cpp
    src/memory_pool.cpp
)

target_include_directories(tests PRIVATE include)
target_link_libraries(tests gtest gtest_main)