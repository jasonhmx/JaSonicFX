add_executable(
    test_jsfx

    src/TestJSFX.cpp
)

target_link_libraries(
    test_jsfx PRIVATE
    lib_jsfx
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_jsfx)