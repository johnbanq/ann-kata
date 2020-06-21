cmake_minimum_required(VERSION 3.10)
project(ann-demo)

#import dependencies
add_subdirectory(lib EXCLUDE_FROM_ALL)

#subproject: algorithm
add_subdirectory(ann-algorithm)