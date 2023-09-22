cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 11)
# aux_source_directory(${PROJECT_SOURCE_DIR} app) #①
file(GLOB app ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(D:/software/Boost/boost_1_82_0/boost_1_82_0)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(D:/software/Boost/boost_1_82_0/boost_1_82_0/stage/lib)
#link_libraries()

add_library(test1 STATIC ${app})
add_executable(untitled main.cpp
       # log.h
        src/log.cpp
        src/util.cpp
        src/singleton.cpp
        testLog.cpp
        src/config.cpp
)

 #add_executable(untitled ${app}) #①
