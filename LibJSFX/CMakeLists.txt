add_library(
    lib_jsfx STATIC
    ./src/test.cpp
)

target_include_directories(
    lib_jsfx PUBLIC
    ./include
)