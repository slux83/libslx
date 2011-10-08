PROJECT(libslx)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

option(BUILD_TESTS    "Compile tests"     ON)

file(GLOB SLX_SOURCES   src/*.cpp src/concurrent/*.cpp
                        src/global/*.cpp
                        src/*.h
                        src/concurrent/*.h
						src/global/*.h
						src/core/*.h
						src/core/*.cpp)

set(CMAKE_CXX_FLAGS "-Wall -g -Os -pedantic")

if(BUILD_TESTS)
file(GLOB SLX_TESTS_SOURCES test/cppunit/*_test.cpp test/cppunit/*_test.h )
add_executable(RunTest test/testrunner.cpp ${SLX_TESTS_SOURCES} ${SLX_SOURCES})
TARGET_LINK_LIBRARIES(RunTest cppunit pthread)
FOREACH(test ${SLX_TESTS_SOURCES})
        MESSAGE("-- Adding Test case: ${test}")
        GET_FILENAME_COMPONENT(TestName ${test} NAME_WE)
        ADD_TEST(${TestName} RunTest ${TestName})
ENDFOREACH(test)
endif(BUILD_TESTS)

set(CMAKE_BUILD_TYPE Debug)
set(SLX_SOURCES ${SLX_SOURCES})
add_library(slx SHARED ${SLX_SOURCES})



