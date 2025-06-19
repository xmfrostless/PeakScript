
cmake_minimum_required(VERSION 3.30)

project(PeakScript)
set(PeakScript "PeakScript")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# 编译选项
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options("/Wall" "/wd5045" "/wd4711" "/wd4710" "/wd4820" "/wd4514")
	add_compile_options("/EHsc" "/permissive-")
	add_compile_options("/MP" "/Zc:__cplusplus" "/Zc:preprocessor")
	add_compile_options("/source-charset:utf-8")
	# add_compile_options("/MT")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CXXFLAGS "-Wall" "-Wextra" "-pedantic" "-std=c++17" "-O2")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CXXFLAGS "-Wall" "-Wextra" "-pedantic" "-std=c++17" "-O2")
endif()

message("> " ${CMAKE_CXX_COMPILER_ID} " " ${CXXFLAGS})
add_compile_options(${CXXFLAGS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# PeakScript
include_directories("./src")

file(GLOB_RECURSE SOURCES "./src/*.cpp" "./src/*.h")
file(GLOB_RECURSE REMOVE_SOURCES "./src/._*")
list(REMOVE_ITEM SOURCES ${REMOVE_SOURCES})

add_library(${PeakScript} STATIC ${SOURCES})

# cli-peak
include_directories("./cli-peak")
file (GLOB_RECURSE CLI_PEAK_SOURCES "./cli-peak/*.cpp" "./cli-peak/*.h")
file(GLOB_RECURSE REMOVE_CLI_PEAK_SOURCES "./cli-peak/._*")
list(REMOVE_ITEM CLI_PEAK_SOURCES ${REMOVE_CLI_PEAK_SOURCES})
add_executable("peak" ${CLI_PEAK_SOURCES})
target_link_libraries("peak" ${PeakScript})

# test
include_directories("./test")
file (GLOB_RECURSE TEST_SOURCES "./test/*.cpp" "./test/*.h")
file(GLOB_RECURSE REMOVE_TEST_SOURCES "./test/._*")
list(REMOVE_ITEM TEST_SOURCES ${REMOVE_TEST_SOURCES})
add_executable("test" ${TEST_SOURCES})
target_link_libraries("test" ${PeakScript})

