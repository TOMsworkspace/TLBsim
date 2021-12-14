cmake_minimum_required(VERSION 3.17)
project(TLBsim)

set(CMAKE_CXX_STANDARD 11)

include_directories(cache)
include_directories(MMU)
include_directories(util)
include_directories(.)

AUX_SOURCE_DIRECTORY(cache SRC_LIST)
AUX_SOURCE_DIRECTORY(MMU SRC_LIST)
AUX_SOURCE_DIRECTORY(util SRC_LIST)

#message("${SRC_LIST}")

add_executable(Experiment1
        Experiment1.cpp
        ${SRC_LIST}
)

add_executable(Experiment2
        Experiment2.cpp
        ${SRC_LIST}
)
