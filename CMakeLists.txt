cmake_minimum_required(VERSION 3.6)
project(reLib)

if(NOT TARGET gsl)
    add_subdirectory(external/gsl EXCLUDE_FROM_ALL)
endif()


add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE gsl)
target_compile_options(${PROJECT_NAME} INTERFACE "-std=c++1z")
target_compile_options(${PROJECT_NAME} INTERFACE "-Weverything")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-c++98-compat")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-c++98-compat-pedantic")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-missing-prototypes")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-unknown-pragmas")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-unused-parameter")
target_compile_options(${PROJECT_NAME} INTERFACE "-Wno-weak-vtables")
target_compile_options(${PROJECT_NAME} INTERFACE "-march=native")


add_subdirectory(tests)

