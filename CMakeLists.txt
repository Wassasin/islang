cmake_minimum_required(VERSION 2.8)

project(islang)

#include_directories("${PROJECT_BINARY_DIR}")

list(APPEND islang_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/src/")


add_subdirectory(src/islang)

# set(islang_INCLUDE_DIRS ${islang_INCLUDE_DIRS} PARENT_SCOPE)

# file(GLOB_RECURSE ISLANG_SOURCES src/*.cpp)
# file(GLOB_RECURSE ISLANG_HEADERS src/*.hpp)

# find_package(BISON)
# find_package(FLEX)

#BISON_TARGET(islang_parser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/src/parser/parser.cc)
# FLEX_TARGET(islang_scanner src/parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
#ADD_FLEX_BISON_DEPENDENCY(islang_scanner islang_parser)

# include_directories(${CMAKE_CURRENT_BINARY_DIR})
