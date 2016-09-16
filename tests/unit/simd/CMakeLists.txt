cmake_minimum_required(VERSION 3.6)

set(SIMD_UNIT_TEST_NAME "${PROJECT_NAME}_simd_unit")

add_executable(${SIMD_UNIT_TEST_NAME} main.cpp)
target_link_libraries(${SIMD_UNIT_TEST_NAME} ${PROJECT_NAME})
target_link_libraries(${SIMD_UNIT_TEST_NAME} gtest)
