cmake_minimum_required(VERSION 2.8)
project(cpsl)

if(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11")
endif()
find_package(BISON)
find_package(FLEX)

BISON_TARGET(CpslParser FrontEnd/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(CpslScanner FrontEnd/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(CpslScanner CpslParser)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/AST_INC)

set(cpsl_srcs main.cpp)
source_group("" FILES ${cpsl_srcs})

set(frontend_srcs
FrontEnd/FrontEnd.hpp
FrontEnd/FrontEnd.cpp
FrontEnd/ProcessLog.cpp
FrontEnd/ProcessLog.hpp
${BISON_CpslParser_OUTPUTS}
${FLEX_CpslScanner_OUTPUTS}
)
source_group("FrontEnd" FILES ${frontend_srcs})
list(APPEND cpsl_srcs ${frontend_srcs})

set(ast_srcs
AST_INC/AST/Program.hpp
AST_INC/AST/Function.hpp
AST_INC/AST/SymbolTable.hpp
AST_INC/AST/ThreeAddressInstruction.hpp
AST_INC/AST/BasicBlock.hpp
)
source_group("FrontEnd" FILES ${frontend_srcs})
list(APPEND cpsl_srcs ${frontend_srcs})

set(backend_srcs
BackEnd/BackEnd.hpp
BackEnd/BackEnd.cpp
)
source_group("BackEnd" FILES ${backend_srcs})
list(APPEND cpsl_srcs ${backend_srcs})

set(optimization_srcs
Optimizations/Optimizer.hpp
Optimizations/Optimizer.cpp
)
source_group("Optimizations" FILES ${optimization_srcs})
list(APPEND cpsl_srcs ${optimization_srcs})

add_executable(cpsl ${cpsl_srcs})

target_link_libraries(cpsl ${FLEX_LIBRARIES} ${BISON_LIBRARIES})


