
add_subdirectory(external/benchmark)

add_executable(${PROJECT_NAME}_bench benchmark.cpp ../include)
target_link_libraries(${PROJECT_NAME}_bench ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_bench benchmark)
