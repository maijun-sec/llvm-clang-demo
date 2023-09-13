CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_NAME clang_ast_basic_traverse)

PROJECT(clang_ast_basic_traverse)

ADD_EXECUTABLE(clang_ast_basic_traverse ASTVisitorExample.cpp)

TARGET_LINK_LIBRARIES(clang_ast_basic_traverse
    clangAST
    clangBasic
    clangFrontend
    clangTooling)
