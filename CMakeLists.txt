cmake_minimum_required(VERSION 2.8)
project(cpsl)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(CpslParser FrontEnd/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(CpslScanner FrontEnd/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(CpslScanner CpslParser)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/AST_INC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")
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
  AST_INC/AST/Function.cpp
  AST_INC/AST/Function.hpp
  AST_INC/AST/LValue.hpp
  AST_INC/AST/Program.hpp
  AST_INC/AST/StringTable.cpp
  AST_INC/AST/StringTable.hpp
  AST_INC/AST/SymbolTable.cpp
  AST_INC/AST/SymbolTable.hpp
  AST_INC/AST/ThreeAddressInstruction.cpp
  AST_INC/AST/ThreeAddressInstruction.hpp
  AST_INC/AST/Type.cpp
  AST_INC/AST/Type.hpp
)

source_group("FrontEnd" FILES ${ast_srcs})
list(APPEND cpsl_srcs ${ast_srcs})

set(statement_srcs
  AST_INC/AST/Statements/Assignment.cpp
  AST_INC/AST/Statements/Assignment.hpp
  AST_INC/AST/Statements/Call.cpp
  AST_INC/AST/Statements/Call.hpp
  AST_INC/AST/Statements/For.cpp
  AST_INC/AST/Statements/For.hpp
  AST_INC/AST/Statements/If.cpp
  AST_INC/AST/Statements/If.hpp
  AST_INC/AST/Statements/Read.cpp
  AST_INC/AST/Statements/Read.hpp
  AST_INC/AST/Statements/Repeat.cpp
  AST_INC/AST/Statements/Repeat.hpp
  AST_INC/AST/Statements/Return.cpp
  AST_INC/AST/Statements/Return.hpp
  AST_INC/AST/Statements/Statement.cpp
  AST_INC/AST/Statements/Statement.hpp
  AST_INC/AST/Statements/Stop.cpp
  AST_INC/AST/Statements/Stop.hpp
  AST_INC/AST/Statements/While.cpp
  AST_INC/AST/Statements/While.hpp
  AST_INC/AST/Statements/Write.cpp
  AST_INC/AST/Statements/Write.hpp
  )
source_group("FrontEnd/Statements" FILES ${statement_srcs})
list(APPEND cpsl_srcs ${statement_srcs})

set(expr_srcs
  AST_INC/AST/Expressions/AdditionExpression.cpp
  AST_INC/AST/Expressions/AdditionExpression.hpp
  AST_INC/AST/Expressions/AndExpression.cpp
  AST_INC/AST/Expressions/AndExpression.hpp
  AST_INC/AST/Expressions/CallExpression.cpp
  AST_INC/AST/Expressions/CallExpression.hpp
  AST_INC/AST/Expressions/ChrExpression.cpp
  AST_INC/AST/Expressions/ChrExpression.hpp
  AST_INC/AST/Expressions/DivExpression.cpp
  AST_INC/AST/Expressions/DivExpression.hpp
  AST_INC/AST/Expressions/EqExpression.cpp
  AST_INC/AST/Expressions/EqExpression.hpp
  AST_INC/AST/Expressions/Expression.cpp
  AST_INC/AST/Expressions/Expression.hpp
  AST_INC/AST/Expressions/GtExpression.cpp
  AST_INC/AST/Expressions/GtExpression.hpp
  AST_INC/AST/Expressions/GteExpression.cpp
  AST_INC/AST/Expressions/GteExpression.hpp
  AST_INC/AST/Expressions/LiteralExpression.cpp
  AST_INC/AST/Expressions/LiteralExpression.hpp
  AST_INC/AST/Expressions/LoadExpression.cpp
  AST_INC/AST/Expressions/LoadExpression.hpp
  AST_INC/AST/Expressions/LtExpression.cpp
  AST_INC/AST/Expressions/LtExpression.hpp
  AST_INC/AST/Expressions/LteExpression.cpp
  AST_INC/AST/Expressions/LteExpression.hpp
  AST_INC/AST/Expressions/ModExpression.cpp
  AST_INC/AST/Expressions/ModExpression.hpp
  AST_INC/AST/Expressions/MultExpression.cpp
  AST_INC/AST/Expressions/MultExpression.hpp
  AST_INC/AST/Expressions/NeqExpression.cpp
  AST_INC/AST/Expressions/NeqExpression.hpp
  AST_INC/AST/Expressions/NotExpression.cpp
  AST_INC/AST/Expressions/NotExpression.hpp
  AST_INC/AST/Expressions/OrExpression.cpp
  AST_INC/AST/Expressions/OrExpression.hpp
  AST_INC/AST/Expressions/OrdExpression.cpp
  AST_INC/AST/Expressions/OrdExpression.hpp
  AST_INC/AST/Expressions/PredecessorExpression.cpp
  AST_INC/AST/Expressions/PredecessorExpression.hpp
  AST_INC/AST/Expressions/StringExpression.cpp
  AST_INC/AST/Expressions/StringExpression.hpp
  AST_INC/AST/Expressions/SubtractExpression.cpp
  AST_INC/AST/Expressions/SubtractExpression.hpp
  AST_INC/AST/Expressions/SuccessorExpression.cpp
  AST_INC/AST/Expressions/SuccessorExpression.hpp
  AST_INC/AST/Expressions/UnaryMinusExpression.cpp
  AST_INC/AST/Expressions/UnaryMinusExpression.hpp
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


