add_executable(
    test_jsfx
    src/main.cpp
)

target_link_libraries(
    test_jsfx PRIVATE
    lib_jsfx
)