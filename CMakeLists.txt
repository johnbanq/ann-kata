cmake_minimum_required(VERSION 3.10)
project(ann-demo)

#import dependencies
add_subdirectory(lib)

#subproject: algorithm
add_subdirectory(ann-algorithm)