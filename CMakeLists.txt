cmake_minimum_required(VERSION 3.15)
project(MemoryPoolProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable
add_executable(memory_pool_demo
    src/main.cpp
    src/memory_pool.cpp
)

target_include_directories(memory_pool_demo PRIVATE include)

# Удаляем старые файлы GoogleTest если они есть
if(EXISTS ${CMAKE_BINARY_DIR}/_deps)
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/_deps)
endif()

# GoogleTest с явным указанием репозитория
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# Для Windows может потребоваться отключить SSL проверку
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Tests
add_executable(tests
    tests/tests.cpp
    src/memory_pool.cpp
)

target_include_directories(tests PRIVATE include)
target_link_libraries(tests gtest gtest_main)