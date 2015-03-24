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

enable_testing()
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/AST_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/log
  ${CMAKE_CURRENT_SOURCE_DIR}/Optimizations
  ${CMAKE_CURRENT_SOURCE_DIR}/FrontEnd
  ${CMAKE_CURRENT_SOURCE_DIR}/BackEnd
  ${CMAKE_CURRENT_SOURCE_DIR}/Testing
  ${Boost_INCLUDE_DIRS}
  ${WINDOWS_GNU}
)

set(EXTRA_COMPILE_FLAGS "-g -std=c++0x")
#set(LCOV_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS} ${LCOV_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LCOV_FLAGS}")

set(main_srcs main.cpp)
source_group("Main" FILES ${cpsl_srcs})

set(frontend_srcs
  FrontEnd/FrontEnd.cpp
  ${BISON_CpslParser_OUTPUTS}
  ${FLEX_CpslScanner_OUTPUTS}
)
source_group("FrontEnd" FILES ${frontend_srcs})
set(cpsl_srcs ${frontend_srcs})

set(ast_srcs
  AST_INC/AST/BasicBlock.cpp
  AST_INC/AST/Function.cpp
  AST_INC/AST/LValue.cpp
  AST_INC/AST/StringTable.cpp
  AST_INC/AST/SymbolTable.cpp
  AST_INC/AST/ThreeAddressInstruction.cpp
  AST_INC/AST/Type.cpp
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
  AST_INC/AST/Statements/Call.cpp
  AST_INC/AST/Statements/For.cpp
  AST_INC/AST/Statements/If.cpp
  AST_INC/AST/Statements/Read.cpp
  AST_INC/AST/Statements/Repeat.cpp
  AST_INC/AST/Statements/Return.cpp
  AST_INC/AST/Statements/Statement.cpp
  AST_INC/AST/Statements/Stop.cpp
  AST_INC/AST/Statements/While.cpp
  AST_INC/AST/Statements/Write.cpp
)
source_group("FrontEnd/Statements" FILES ${statement_srcs})
list(APPEND cpsl_srcs ${statement_srcs})

set(expr_srcs
  AST_INC/AST/Expressions/AdditionExpression.cpp
  AST_INC/AST/Expressions/AndExpression.cpp
  AST_INC/AST/Expressions/CallExpression.cpp
  AST_INC/AST/Expressions/ChrExpression.cpp
  AST_INC/AST/Expressions/DivExpression.cpp
  AST_INC/AST/Expressions/EqExpression.cpp
  AST_INC/AST/Expressions/Expression.cpp
  AST_INC/AST/Expressions/GtExpression.cpp
  AST_INC/AST/Expressions/GteExpression.cpp
  AST_INC/AST/Expressions/LiteralExpression.cpp
  AST_INC/AST/Expressions/LoadExpression.cpp
  AST_INC/AST/Expressions/LtExpression.cpp
  AST_INC/AST/Expressions/LteExpression.cpp
  AST_INC/AST/Expressions/MemoryAccessExpression.cpp
  AST_INC/AST/Expressions/ModExpression.cpp
  AST_INC/AST/Expressions/MultExpression.cpp
  AST_INC/AST/Expressions/NeqExpression.cpp
  AST_INC/AST/Expressions/NotExpression.cpp
  AST_INC/AST/Expressions/OrExpression.cpp
  AST_INC/AST/Expressions/OrdExpression.cpp
  AST_INC/AST/Expressions/PredecessorExpression.cpp
  AST_INC/AST/Expressions/StringExpression.cpp
  AST_INC/AST/Expressions/SubtractExpression.cpp
  AST_INC/AST/Expressions/SuccessorExpression.cpp
  AST_INC/AST/Expressions/UnaryMinusExpression.cpp
)
source_group("FrontEnd/Expressions" FILES ${expr_srcs})
list(APPEND cpsl_srcs ${expr_srcs})

set(backend_srcs
  BackEnd/BackEnd.cpp
  BackEnd/Allocation.cpp
)
source_group("BackEnd" FILES ${backend_srcs})
list(APPEND cpsl_srcs ${backend_srcs})

set(optimization_srcs
  Optimizations/Optimizer.cpp
  Optimizations/MaximizeBlocks/MaximizeBlocks.cpp
  Optimizations/SuperBlockClone/SuperBlockClone.cpp
  Optimizations/FlowGraph.cpp
)
source_group("Optimizations" FILES ${optimization_srcs})
list(APPEND cpsl_srcs ${optimization_srcs})

list(APPEND cpsl_srcs ${unittests})

file(GLOB unittests Testing/UnitTests/*.cpp)
set(test_srcs ${unittests}
    Testing/main.cpp
    Testing/ProgramFactory.cpp
    ${BISON_CpslParser_OUTPUTS}
    ${FLEX_CpslScanner_OUTPUTS}
    )


set(CMAKE_INSTALL_RPATH ".")
add_library(cpsl_common STATIC ${cpsl_srcs})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(cpsl_common PROPERTIES MACOSX_RPATH "@loader_path/../lib")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_executable(cpsl ${main_srcs})
add_executable(cpsl_test ${test_srcs})

target_link_libraries(cpsl_common ${FLEX_LIBRARIES} ${BISON_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(cpsl cpsl_common)
target_link_libraries(cpsl_test cpsl_common)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log/log.conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/log)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tester/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tester)

add_test(UnitTests cpsl_test)

find_program(BASH_PROGRAM bash)
if (BASH_PROGRAM)
    add_test(NAME IntegrationTest COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_BINARY_DIR}/tester/tester.sh)
endif (BASH_PROGRAM)
