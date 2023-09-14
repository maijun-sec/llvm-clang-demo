FILE(GLOB_RECURSE srcs "*.cpp")

SET(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/FirstCheckerGroup.exports)
ADD_LIBRARY(FirstCheckerGroup MODULE ${srcs})

IF (LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
  TARGET_LINK_LIBRARIES(FirstCheckerGroup PRIVATE
      clangAnalysis
      clangAST
      clangStaticAnalyzerCore
      LLVMSupport
      )
ENDIF ()
