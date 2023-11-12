add_executable(
    test_jsfx
    main.cpp

    src/TestFIFOBuffer.cpp
)

target_link_libraries(
    test_jsfx PRIVATE
    lib_jsfx
    GTest::gtest_main
)