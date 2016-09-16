cmake_minimum_required(VERSION 3.6)

set(RE_BENCHMARK_TEST_NAME ${PROJECT_NAME}_benchmark)
message(${RE_BENCHMARK_TEST_NAME})

add_executable(${RE_BENCHMARK_TEST_NAME} benchmark.cpp)
target_link_libraries(${RE_BENCHMARK_TEST_NAME} ${PROJECT_NAME})

target_link_libraries(${RE_BENCHMARK_TEST_NAME} benchmark)
target_compile_options(${RE_BENCHMARK_TEST_NAME} PRIVATE "-Wno-global-constructors")
target_compile_options(${RE_BENCHMARK_TEST_NAME} PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-O3>")

add_test(${RE_BENCHMARK_TEST_NAME} ${RE_BENCHMARK_TEST_NAME})
