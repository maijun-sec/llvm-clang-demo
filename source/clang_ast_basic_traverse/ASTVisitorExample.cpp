#include <iostream>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class FindNamedClassVisitor
    : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
  FindNamedClassVisitor() {}

  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    if (Declaration->getQualifiedNameAsString() == "n::m::C") {
      FullSourceLoc FullLocation = Declaration->getASTContext().getFullLoc(Declaration->getBeginLoc());
      if (FullLocation.isValid())
        llvm::outs() << "Found declaration at "
                     << FullLocation.getSpellingLineNumber() << ":"
                     << FullLocation.getSpellingColumnNumber() << "\n";
    }
    return true;
  }
};

class FindNamedClassConsumer : public clang::ASTConsumer {
public:
  FindNamedClassConsumer() {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor->TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  std::unique_ptr<FindNamedClassVisitor> Visitor;
};

/// 例子改自：https://clang.llvm.org/docs/RAVFrontendAction.html
int main(int argc, char **argv) {
  const std::string Code = "namespace n { namespace m { class C {}; } }";

  std::vector<std::string> Args;
  Args.emplace_back("-xc++");
  std::unique_ptr<ASTUnit> Unit = tooling::buildASTFromCodeWithArgs(Code, Args, "test.c");

  // 执行上面的 ASTConsumer 操作
  std::unique_ptr<FindNamedClassConsumer> Consumer = std::make_unique<FindNamedClassConsumer>();
  Consumer->HandleTranslationUnit(Unit->getASTContext());
}
