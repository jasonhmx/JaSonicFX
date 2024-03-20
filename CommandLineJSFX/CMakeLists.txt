add_executable(
    command_line_jsfx
    src/main.cpp
)

target_link_libraries(
    command_line_jsfx PRIVATE
    lib_jsfx
)

target_include_directories(
    command_line_jsfx PUBLIC
    ./include
)