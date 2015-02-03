cmake_minimum_required(VERSION 2.8)

project(islang)

include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE ISLANG_SOURCES src/*.cpp)
file(GLOB_RECURSE ISLANG_HEADERS src/*.hpp)

add_executable(islang
	${ISLANG_SOURCES}
	${ISLANG_HEADERS}
)

add_definitions("-Wall -Wextra -Weffc++ -std=c++11 -pedantic -g -O3")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(Boost COMPONENTS system program_options regex filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
                    
target_link_libraries(islang
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      ${Boost_REGEX_LIBRARY}
                      ${Boost_FILESYSTEM_LIBRARY})
