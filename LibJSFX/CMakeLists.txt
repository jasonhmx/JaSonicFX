add_library(
    lib_jsfx STATIC
    ./src/FIFOBuffer.cpp
)

target_include_directories(
    lib_jsfx PUBLIC
    ./include
)