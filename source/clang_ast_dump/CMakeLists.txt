CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_NAME clang_ast_dump)

PROJECT(clang_ast_dump)

ADD_EXECUTABLE(clang_ast_dump ASTDump.cpp)

TARGET_LINK_LIBRARIES(clang_ast_dump
    clangAST
    clangBasic
    clangFrontend
    clangTooling)
