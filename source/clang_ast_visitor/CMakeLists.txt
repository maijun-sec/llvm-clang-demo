CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_NAME clang_ast_visitor)

PROJECT(clang_ast_visitor)

ADD_EXECUTABLE(clang_ast_visitor ASTVisitorExample.cpp)

TARGET_LINK_LIBRARIES(clang_ast_visitor
    clangAST
    clangBasic
    clangFrontend
    clangTooling)
