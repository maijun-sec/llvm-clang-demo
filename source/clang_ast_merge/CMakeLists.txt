CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_NAME clang_ast_merge)

PROJECT(clang_ast_merge)

ADD_EXECUTABLE(clang_ast_merge ASTMerge.cpp)

TARGET_LINK_LIBRARIES(clang_ast_merge
    clangAST
    clangBasic
    clangFrontend
    clangTooling)
