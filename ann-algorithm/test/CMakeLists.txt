

add_executable(
    brute-force-test
    test/test-main.cpp
    test/brute-force-test.cpp
)
add_test(
    ANN::Algorithm::Test::BruteForce
    brute-force-test
)

target_link_libraries(brute-force-test PRIVATE ann-algorithm)
target_link_libraries(brute-force-test PRIVATE catch2)
