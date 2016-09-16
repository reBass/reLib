
add_subdirectory(external/benchmark EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME}_bench benchmark.cpp ../include)
target_link_libraries(${PROJECT_NAME}_bench ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_bench benchmark)
