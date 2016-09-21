cmake_minimum_required(VERSION 3.6)

if(NOT TARGET gtest)
    set(gtest_build_tests OFF CACHE BOOL "enable gtest's tests")
    add_subdirectory(external/googletest/googletest EXCLUDE_FROM_ALL)

    target_sources(gtest PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/external/googletest/googletest/include)
    target_compile_options(gtest PUBLIC "-Wno-undef")
    target_compile_options(gtest PUBLIC "-Wno-deprecated")
    target_compile_options(gtest PUBLIC "-Wno-missing-noreturn")
    target_compile_options(gtest PUBLIC "-Wno-shift-sign-overflow")
    target_compile_options(gtest PUBLIC "-Wno-used-but-marked-unused")
endif()
if(NOT TARGET benchmark)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "enable benchmark tests")
    add_subdirectory(external/benchmark EXCLUDE_FROM_ALL)

    target_sources(benchmark PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/external/benchmark/include)
endif()

add_subdirectory(benchmark)
add_subdirectory(unit/simd)
