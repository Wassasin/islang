find_package(BISON)
find_package(FLEX)

FLEX_TARGET(islang_scanner lexer.x ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
BISON_TARGET(islang_parser parser_lr.y ${CMAKE_CURRENT_BINARY_DIR}/parser_lr.cc)
ADD_FLEX_BISON_DEPENDENCY(islang_scanner islang_parser)

# Add the target directory of the islang_parser for our static target
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${islang_INCLUDE_DIRS})
add_library(islang_parser STATIC parser.cpp ${FLEX_islang_scanner_OUTPUTS} ${BISON_islang_parser_OUTPUTS})

set(islang_LIBRARIES ${islang_LIBRARIES} islang_parser ${FLEX_LIBRARIES} ${BISON_LIBRARIES} PARENT_SCOPE)
