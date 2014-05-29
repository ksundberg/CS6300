cmake_minimum_required(VERSION 2.8)
project(cpsl)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(CpslParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(CpslScanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(CpslScanner CpslParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(cpsl_srcs
Expressions.cpp
Expressions.hpp
main.cpp
ProcessLog.cpp
ProcessLog.hpp
RegisterPool.cpp
RegisterPool.hpp
StringTable.cpp
StringTable.hpp
SymbolTable.cpp
SymbolTable.hpp
Type.hpp
${BISON_CpslParser_OUTPUTS}
${FLEX_CpslScanner_OUTPUTS}

)
add_executable(cpsl ${cpsl_srcs})
source_group(cpsl FILES ${cpsl_srcs})

target_link_libraries(cpsl ${FLEX_LIBRARIES} ${BISON_LIBRARIES})


