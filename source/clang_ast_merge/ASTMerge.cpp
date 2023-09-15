#include <iostream>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTDumperUtils.h"
#include "clang/AST/ASTImporter.h"
#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

int main(int argc, char **argv) {
  const std::string Code1 = R"S(
    int main() {
        int x = 4;
        return x;
    }
)S";

  const std::string Code2 = R"CODE(
class Dog {
public:
    void say() {
        // ...
    }
};

template<typename T>
bool isSmaller(T fir, T sec){
    return true;
}

int test(int x, int y){
    int result = 0;
    if (x > y) {
        result = x - y;
    } else {
        result = y - x;
    }
    return result;
}
)CODE";

  std::vector<std::string> Args;
  Args.emplace_back("-xc++");

  std::unique_ptr<ASTUnit> Unit1 = tooling::buildASTFromCodeWithArgs(Code1, Args, "test1.cpp");
  std::unique_ptr<ASTUnit> Unit2 = tooling::buildASTFromCodeWithArgs(Code2, Args, "test2.cpp");

  ASTImporter Importer(Unit1->getASTContext(), Unit1->getFileManager(), Unit2->getASTContext(), Unit2->getFileManager(), true);

  // CAUTION：这里，我们直接将 Unit2 中全部的内容，都 Merge 到 Unit1 中
  // 如果自己只希望 Merge 其中的一部分，可以筛选一些处理（筛选可以通过 AST遍历，或者是 ASTMatcher 实现）
  auto ImportedOrErr = Importer.Import(Unit2->getASTContext().getTranslationUnitDecl());
  if (!ImportedOrErr) {
    llvm::errs() << "Error\n";
    return -1;
  }

  if (auto Err = Importer.ImportDefinition(Unit2->getASTContext().getTranslationUnitDecl())) {
    llvm::errs() << "Error\n";
    return -1;
  }

  llvm::outs() << "================================> Stats: \n";
  Unit1->getASTContext().PrintStats();

  llvm::outs() << "================================> AST: \n";
  std::unique_ptr<ASTConsumer> Dumper = CreateASTDumper(nullptr, "", true, true, true, true, ADOF_Default);
  Dumper->HandleTranslationUnit(Unit1->getASTContext());
}
