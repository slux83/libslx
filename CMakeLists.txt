PROJECT(libslx)
cmake_minimum_required(VERSION 2.8)

file(GLOB SLX_SOURCES   src/*.cpp src/concurrent/*.cpp
                        src/global/*.cpp
                        src/*.h
                        src/concurrent/*.h
                        src/global/*.h)

set(CMAKE_CXX_FLAGS "-Wall -O0 -pedantic")


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(CMAKE_BUILD_TYPE Debug)
set(SLX_SOURCES ${SLX_SOURCES})
add_library(slx SHARED ${SLX_SOURCES})



