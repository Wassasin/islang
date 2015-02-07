# First level dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/contrib/wubwubcmake/modules")

find_package(Boost COMPONENTS system program_options REQUIRED)
list(APPEND islang_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND islang_LIBRARIES ${Boost_LIBRARIES})

# Target definitions
add_definitions("-Wall -Wextra -Weffc++ -Wno-deprecated-register -std=c++11 -pedantic -g -O3")

# Subdirectories
add_subdirectory(common)
add_subdirectory(parser)

# Main targets
add_executable(islang
	main.cpp
)

include_directories(${islang_INCLUDE_DIRS})
target_link_libraries(islang ${islang_LIBRARIES})
