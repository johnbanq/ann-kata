
# Test: brute-force Works
add_executable(
    brute-force-test
    test/test-main.cpp
    test/brute-force-test.cpp
)
add_test(
    ANN::Algorithm::BruteForce
    brute-force-test
)

target_link_libraries(brute-force-test PRIVATE ann-algorithm)
target_link_libraries(brute-force-test PRIVATE catch2)

# Test: undirected_grpah works
add_executable(
    graph-test
    test/test-main.cpp
    test/graph-test.cpp
)
add_test(
    ANN::Algorithm::Graph
    graph-test
)

target_link_libraries(graph-test PRIVATE ann-algorithm)
target_link_libraries(graph-test PRIVATE catch2)

# Test: hnsw works
add_executable(
    hnsw-test
    test/test-main.cpp
    test/hnsw-test.cpp
    test/hnsw-test-select-neighbor.cpp
)
add_test(
    ANN::Algorithm::HSNW
    hnsw-test
)

target_link_libraries(hnsw-test PRIVATE ann-algorithm)
target_link_libraries(hnsw-test PRIVATE catch2)

# Test: hnsw vs brute force
add_executable(
    hnsw-cross-test
    test/test-main.cpp
    test/hnsw-test-cross-valid.cpp
)
add_test(
    ANN::Algorithm::HSNW::CrossValidation
    hnsw-cross-test
)

target_link_libraries(hnsw-cross-test PRIVATE ann-algorithm)
target_link_libraries(hnsw-cross-test PRIVATE catch2)
