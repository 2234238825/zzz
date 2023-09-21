cmake_minimum_required(VERSION 3.26)
project(test1)

set(CMAKE_CXX_STANDARD 11)
aux_source_directory(${PROJECT_SOURCE_DIR} app) #①
include_directories(D:/software/Boost/boost_1_82_0/boost_1_82_0)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(D:/software/Boost/boost_1_82_0/boost_1_82_0/stage/lib)
add_executable(untitled ${app}) #①
