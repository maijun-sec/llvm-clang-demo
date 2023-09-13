#include <iostream>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

/// 基于 RecursiveASTVisitor 实现的 AST 递归遍历
class ExampleASTVisitor
    : public RecursiveASTVisitor<ExampleASTVisitor> {
  // 注意：在开发递归的AST遍历时，这里的 base 非常重要，因为每个 TraverseXX 在执行完，最后都需要调用 base::TraverseXX(...)
  // 这样才能递归下去。如下面的 TraverseCXXBaseSpecifier 和 TraverseStmt
  typedef RecursiveASTVisitor<ExampleASTVisitor> base;
public:
  ExampleASTVisitor() {}

  bool TraverseCXXBaseSpecifier(const CXXBaseSpecifier &Base) {
    llvm::outs() << "CXX Base Specifier: " << Base.getType() << "\n";
    return base::TraverseCXXBaseSpecifier(Base);
  }

  bool TraverseStmt(Stmt *S, DataRecursionQueue *Queue = nullptr) {
    llvm::outs() << "Stmt: ";
    S->dump();
    return base::TraverseStmt(S, Queue);
  }
};

class ExampleASTConsumer : public clang::ASTConsumer {
public:
  ExampleASTConsumer() {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor->TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  std::unique_ptr<ExampleASTVisitor> Visitor;
};

int main(int argc, char **argv) {
  const std::string Code = R"CODE(
class Animal {};

class Dog : public Animal {};

void test1(){
    int x = 4;
    int y = x + 2;
}
)CODE";

  std::vector<std::string> Args;
  Args.emplace_back("-xc++");
  std::unique_ptr<ASTUnit> Unit = tooling::buildASTFromCodeWithArgs(Code, Args, "test.c");

  // 执行上面的 ASTConsumer 操作
  std::unique_ptr<ExampleASTConsumer> Consumer = std::make_unique<ExampleASTConsumer>();
  Consumer->HandleTranslationUnit(Unit->getASTContext());
}
