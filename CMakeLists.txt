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
AST_INC/AST/BasicBlock.hpp
AST_INC/AST/Function.hpp
AST_INC/AST/Program.hpp
AST_INC/AST/SymbolTable.hpp
AST_INC/AST/ThreeAddressInstruction.hpp
AST_INC/AST/Type.cpp
AST_INC/AST/Type.hpp
)
source_group("FrontEnd" FILES ${ast_srcs})
list(APPEND cpsl_srcs ${ast_srcs})

set(expr_srcs
AST_INC/AST/AdditionExpression.cpp
AST_INC/AST/AdditionExpression.hpp
AST_INC/AST/AndExpression.cpp
AST_INC/AST/AndExpression.hpp
AST_INC/AST/CallExpression.cpp
AST_INC/AST/CallExpression.hpp
AST_INC/AST/ChrExpression.cpp
AST_INC/AST/ChrExpression.hpp
AST_INC/AST/DivExpression.cpp
AST_INC/AST/DivExpression.hpp
AST_INC/AST/EqExpression.cpp
AST_INC/AST/EqExpression.hpp
AST_INC/AST/Expression.hpp
AST_INC/AST/GtExpression.cpp
AST_INC/AST/GtExpression.hpp
AST_INC/AST/GteExpression.cpp
AST_INC/AST/GteExpression.hpp
AST_INC/AST/LiteralExpression.cpp
AST_INC/AST/LiteralExpression.hpp
AST_INC/AST/LtExpression.cpp
AST_INC/AST/LtExpression.hpp
AST_INC/AST/LteExpression.cpp
AST_INC/AST/LteExpression.hpp
AST_INC/AST/ModExpression.cpp
AST_INC/AST/ModExpression.hpp
AST_INC/AST/MultExpression.cpp
AST_INC/AST/MultExpression.hpp
AST_INC/AST/NeqExpression.cpp
AST_INC/AST/NeqExpression.hpp
AST_INC/AST/NotExpression.cpp
AST_INC/AST/NotExpression.hpp
AST_INC/AST/OrExpression.cpp
AST_INC/AST/OrExpression.hpp
AST_INC/AST/OrdExpression.cpp
AST_INC/AST/OrdExpression.hpp
AST_INC/AST/PredecessorExpression.cpp
AST_INC/AST/PredecessorExpression.hpp
AST_INC/AST/StringExpression.cpp
AST_INC/AST/StringExpression.hpp
AST_INC/AST/StringTable.cpp
AST_INC/AST/StringTable.hpp
AST_INC/AST/SubtractExpression.cpp
AST_INC/AST/SubtractExpression.hpp
AST_INC/AST/SuccessorExpression.cpp
AST_INC/AST/SuccessorExpression.hpp
AST_INC/AST/UnaryMinusExpression.cpp
AST_INC/AST/UnaryMinusExpression.hpp
)
source_group("FrontEnd/Expressions" FILES ${expr_srcs})
list(APPEND cpsl_srcs ${expr_srcs})

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


