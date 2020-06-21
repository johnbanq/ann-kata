project(anndemo-algorithm)

# library definition #

file(GLOB_RECURSE ANN_ALGORITHM_SRC src/*.cpp)
add_library(
    ann-algorithm
    STATIC
    ${ANN_ALGORITHM_SRC}
)

target_include_directories(ann-algorithm PUBLIC include)

target_compile_features(ann-algorithm PUBLIC cxx_std_17)

# test definition #

enable_testing()

include(test/CMakeLists.txt)