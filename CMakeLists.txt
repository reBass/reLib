cmake_minimum_required(VERSION 3.11)
project(reLib)

if(NOT TARGET gsl)
    add_subdirectory(external/gsl EXCLUDE_FROM_ALL)
endif()


add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE gsl)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

enable_testing()
add_subdirectory(tests)

