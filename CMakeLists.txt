cmake_minimum_required(VERSION 2.8)

project(islang)

list(APPEND islang_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/src/")

add_subdirectory(src/islang)
