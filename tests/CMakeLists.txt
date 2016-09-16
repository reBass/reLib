cmake_minimum_required(VERSION 3.6)

add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
target_sources(gtest PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/external/googletest/googletest/include)

add_subdirectory(external/benchmark EXCLUDE_FROM_ALL)
target_sources(benchmark PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/external/benchmark/include)

enable_testing()

add_subdirectory(benchmark)
add_subdirectory(unit/simd)
