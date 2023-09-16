# llvm-clang-demo

A collection of examples about llvm and clang.


cmake arguments:
``` cmake
-DLLVM_DIR=${llvm-install-path}/lib/cmake/llvm
-DClang_DIR=${llvm-install-path}/lib/cmake/clang
```

all testcases are tested use clang 16.0.0

All Examples:
- [source/clang_ast_generate](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/clang_ast_generate)  -- generate clang ast use `tooling::buildASTFromCodeWithArgs()`, and dump clang AST using `CreateASTDumper` API
- [source/clang_ast_dump](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/clang_ast_dump)  -- generate clang ast use `ASTFrontendAction`
- [source/clang_ast_merge](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/clang_ast_merge)  -- merge two cpp ast as one using `ASTImporter`
- [source/clang_ast_traverse](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/clang_ast_basic_traverse))  -- visit the clang ast using `RecursiveASTVisitor`
- [source/clang_ast_visitor](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/clang_ast_visitor)  -- visit the clang ast using `RecursiveASTVisitor`
- [source/clang_ast_cfg](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/clang_ast_cfg)  -- generate clang ast level cfg and traverse the cfg
- [source/clang_ast_callgraph](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/clang_ast_callgraph) -- generate clang ast level call graph
- [source/checkers](https://github.com/maijun-sec/llvm-clang-demo/tree/main/source/checkers)  -- clang static analyzer checker examples using plugin method
