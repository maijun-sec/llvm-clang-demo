CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_NAME clang_ast_callgraph)

PROJECT(clang_ast_callgraph)

ADD_EXECUTABLE(clang_ast_callgraph ASTCallGraphExample.cpp)

TARGET_LINK_LIBRARIES(clang_ast_callgraph
    clangAST
    clangBasic
    clangFrontend
    clangTooling)
