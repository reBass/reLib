
add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
add_subdirectory(external/benchmark EXCLUDE_FROM_ALL)
target_sources(benchmark PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/benchmark/include)

add_executable(${PROJECT_NAME}_bench benchmark.cpp)
target_link_libraries(${PROJECT_NAME}_bench ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}_bench googletest)
target_link_libraries(${PROJECT_NAME}_bench benchmark)
target_compile_options(${PROJECT_NAME}_bench PRIVATE "-Wno-global-constructors")
target_compile_options(${PROJECT_NAME}_bench PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-O3>")
