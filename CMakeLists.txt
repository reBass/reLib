cmake_minimum_required(VERSION 3.6)
project(reLib)

if(NOT TARGET gsl)
    add_subdirectory(external/gsl)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE gsl)
target_compile_options(${PROJECT_NAME} INTERFACE "-std=c++1z")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wall")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wextra")
target_compile_options(${PROJECT_NAME} INTERFACE "-Weffc++")
target_compile_options(${PROJECT_NAME} INTERFACE "-pedantic")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-unknown-pragmas")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-unused-parameter")
target_compile_options(${PROJECT_NAME} INTERFACE "-march=native")


add_subdirectory(tests)

