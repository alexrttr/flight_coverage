cmake_minimum_required(VERSION 3.10)
project(ForestFireTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable
add_executable(forest_fire main.cpp)
target_compile_options(forest_fire PRIVATE -Wall -Wextra -pedantic)

# Catch2-based test
find_package(Catch2 REQUIRED)
add_executable(test_c2 test_c2.cpp)
target_link_libraries(test_c2 PRIVATE Catch2::Catch2WithMain)
target_compile_options(test_c2 PRIVATE -Wall -Wextra -pedantic)

# GoogleTest-based test
find_package(GTest REQUIRED)
add_executable(test_gt test_gt.cpp)
target_link_libraries(test_gt PRIVATE GTest::GTest GTest::Main)
target_compile_options(test_gt PRIVATE -Wall -Wextra -pedantic)

enable_testing()
add_test(NAME Catch2Tests COMMAND test_c2)
add_test(NAME GoogleTestTests COMMAND test_gt)

