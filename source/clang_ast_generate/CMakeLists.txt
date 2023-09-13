CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_NAME clang_ast_generate)

PROJECT(clang_ast_generate)

ADD_EXECUTABLE(clang_ast_generate ASTGenerate.cpp)

TARGET_LINK_LIBRARIES(clang_ast_generate
    clangAST
    clangBasic
    clangFrontend
    clangTooling)
