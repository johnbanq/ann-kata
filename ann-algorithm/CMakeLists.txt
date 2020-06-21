
# library definition #

file(GLOB_RECURSE ANN_ALGORITHM_SRC src)
add_library(
    ann-algorithm
    STATIC
    ${ANN_ALGORITHM_SRC}
)

target_include_directories(ann-algorithm include)

# test definition #

enable_testing()

include(test/CMakeLists.txt)