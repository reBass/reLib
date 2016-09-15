cmake_minimum_required(VERSION 3.6)

add_library(gsl INTERFACE)
target_include_directories(gsl INTERFACE include)
target_compile_options(gsl
    INTERFACE $<$<NOT:$<CONFIG:Debug>>:-DGSL_UNENFORCED_ON_CONTRACT_VIOLATION=1>
)
