cmake_minimum_required(VERSION 2.8)
project(cpsl)

set(Boost_USE_STATIC_LIBS ON)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  find_library(FL_LIBRARY NAMES libfl fl DOC C:/Program Files (x86)/GnuWin32/lib/libfl.a)
  set(WINDOWS_GNU "C:/Program Files (x86)/GnuWin32/include")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

find_package(BISON)
find_package(FLEX)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

bison_target(CpslParser FrontEnd/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
flex_target(CpslScanner FrontEnd/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
add_flex_bison_dependency(CpslScanner CpslParser)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/AST_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/log
  ${WINDOWS_GNU}
  ${Boost_INCLUDE_DIRS}
)

set(EXTRA_COMPILE_FLAGS "-g -std=c++0x")
set(LCOV_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS} ${LCOV_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LCOV_FLAGS}")
set(cpsl_srcs main.cpp)
source_group("" FILES ${cpsl_srcs})

set(frontend_srcs
  FrontEnd/FrontEnd.hpp
  FrontEnd/FrontEnd.cpp
  ${BISON_CpslParser_OUTPUTS}
  ${FLEX_CpslScanner_OUTPUTS}
)
source_group("FrontEnd" FILES ${frontend_srcs})
list(APPEND cpsl_srcs ${frontend_srcs})

set(ast_srcs
  AST_INC/AST/BasicBlock.cpp
  AST_INC/AST/BasicBlock.hpp
  AST_INC/AST/RegisterScope.hpp
  AST_INC/AST/Function.cpp
  AST_INC/AST/Function.hpp
  AST_INC/AST/LValue.hpp
  AST_INC/AST/Program.hpp
  AST_INC/AST/StringTable.cpp
  AST_INC/AST/StringTable.hpp
  AST_INC/AST/Symbol.cpp
  AST_INC/AST/Symbol.hpp
  AST_INC/AST/SymbolTable.cpp
  AST_INC/AST/SymbolTable.hpp
  AST_INC/AST/ThreeAddressInstruction.cpp
  AST_INC/AST/ThreeAddressInstruction.hpp
  AST_INC/AST/Type.cpp
  AST_INC/AST/Type.hpp
)

set(log_srcs
  log/logger.cpp
  log/ProcessLog.cpp
)
source_group("log" FILES ${log_srcs})
list(APPEND cpsl_srcs ${log_srcs})

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
  AST_INC/AST/Expressions/MemoryAccessExpression.cpp
  AST_INC/AST/Expressions/MemoryAccessExpression.hpp
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
  BackEnd/Allocation.hpp
  BackEnd/Allocation.cpp
)
source_group("BackEnd" FILES ${backend_srcs})
list(APPEND cpsl_srcs ${backend_srcs})

set(optimization_srcs
  Optimizations/Optimizer.hpp
  Optimizations/Optimizer.cpp
  Optimizations/MaximizeBlocks/VisitedBlocks.hpp
  Optimizations/MaximizeBlocks/VisitedBlocks.cpp
  Optimizations/MaximizeBlocks/NumParents.hpp
  Optimizations/MaximizeBlocks/NumParents.cpp
  Optimizations/MaximizeBlocks/MaximizeBlocks.hpp
  Optimizations/MaximizeBlocks/MaximizeBlocks.cpp
  Optimizations/FlowGraph.h
  Optimizations/FlowGraph.cpp
)
source_group("Optimizations" FILES ${optimization_srcs})
list(APPEND cpsl_srcs ${optimization_srcs})

add_executable(cpsl ${cpsl_srcs})

target_link_libraries(cpsl ${FLEX_LIBRARIES} ${BISON_LIBRARIES} ${Boost_LIBRARIES})
