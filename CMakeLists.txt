PROJECT(libslx)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

option(BUILD_TESTS       "Compile tests"     ON)
option(BUILD_EXAMPLES    "Compile tests"     ON)

file(GLOB SLX_SOURCES   src/*.cpp src/concurrent/*.cpp
                        src/global/*.cpp
                        src/*.h
                        src/concurrent/*.h
						src/global/*.h
						src/core/*.h
						src/core/*.cpp)

set(CMAKE_CXX_FLAGS "-Wall -g -Os -pedantic")

# Build tests
if(BUILD_TESTS)
	file(GLOB SLX_TESTS_SOURCES test/cppunit/*_test.cpp test/cppunit/*_test.h )
	add_executable(RunTest test/testrunner.cpp ${SLX_TESTS_SOURCES} ${SLX_SOURCES})
	TARGET_LINK_LIBRARIES(RunTest cppunit pthread rt)
	FOREACH(test ${SLX_TESTS_SOURCES})
		GET_FILENAME_COMPONENT(TestName ${test} NAME_WE)
		ADD_TEST(${TestName} RunTest ${TestName})
		MESSAGE("-- Adding Test:    ${TestName}")
	ENDFOREACH(test)
endif(BUILD_TESTS)

# Build examples
if(BUILD_EXAMPLES)
	file(GLOB SLX_EXMPLS example/*)
	FOREACH(example ${SLX_EXMPLS})
		GET_FILENAME_COMPONENT(example_name ${example} NAME_WE)
		add_executable(${example_name} example/${example_name}/${example_name}_main.cpp ${SLX_SOURCES})
		TARGET_LINK_LIBRARIES(${example_name} pthread rt)
		MESSAGE("-- Adding Example: ${example_name}")
	ENDFOREACH(example)
endif(BUILD_EXAMPLES)

set(CMAKE_BUILD_TYPE Debug)
set(SLX_SOURCES ${SLX_SOURCES})
add_library(slx SHARED ${SLX_SOURCES})



