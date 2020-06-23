# Kata: HNSW Algorithm implementation

this is a implementation of the Hierarchical Navigable Small World(HNSW) Algorithm for the ANN problem
based on paper: https://arxiv.org/ftp/arxiv/papers/1603/1603.09320.pdf

## Building

this is a cmake based project, hence:

mkdir build <br/>
cd build <br/>
cmake .. <br/>
cmake --build . <br/>

should work

## Testing

this project uses ctest, you can head to the build/ann-algorithm directory and run ctest to run all tests

The test suite includes basic smoke test and a comparison test between brute-force and HNSW

## Requirements

This project requires C++17 and cmake >= 3.10 to work <br/>
All extra dependencies are included in the lib directory, just clone with --recurse-submodule
