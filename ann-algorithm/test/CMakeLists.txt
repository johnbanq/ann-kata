
add_executable(
    ann-algorithm-test
    test/test-main.cpp
    test/ann-algorithm-test.cpp
)
add_test(
    ANN::Algorithm::Test
    ann-algorithm-test
)

target_link_libraries(ann-algorithm-test PRIVATE ann-algorithm)
target_link_libraries(ann-algorithm-test PRIVATE catch2)
