CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_NAME clang_ast_cfg)

PROJECT(clang_ast_cfg)

ADD_EXECUTABLE(clang_ast_cfg ASTCFGExample.cpp)

TARGET_LINK_LIBRARIES(clang_ast_cfg
    clangAST
    clangBasic
    clangFrontend
    clangTooling)
