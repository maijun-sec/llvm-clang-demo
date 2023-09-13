#include "clang/AST/ASTConsumer.h"
#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/AST/ASTDumperUtils.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class MyASTDumpAction : public ASTFrontendAction {
  llvm::StringRef m_ASTDumpFilter = "";
  bool m_ASTDumpDecls = true;
  bool m_ASTDumpAll = true;
  bool m_ASTDumpLookups = true;
  bool m_ASTDumpDeclTypes = true;
  ASTDumpOutputFormat m_ASTDumpFormat = ADOF_Default;

protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef InFile) override {
    return CreateASTDumper(nullptr , m_ASTDumpFilter,
                           m_ASTDumpDecls, m_ASTDumpAll,
                           m_ASTDumpLookups, m_ASTDumpDeclTypes,
                           m_ASTDumpFormat);
  }

public:
  explicit MyASTDumpAction(llvm::StringRef ASTDumpFilter = "", bool ASTDumpDecls = true, bool ASTDumpAll = true, bool ASTDumpLookups = true, bool ASTDumpDeclTypes = true, ASTDumpOutputFormat ASTDumpFormat = ADOF_Default) : m_ASTDumpFilter(ASTDumpFilter), m_ASTDumpDecls(ASTDumpDecls), m_ASTDumpAll(ASTDumpAll), m_ASTDumpLookups(ASTDumpLookups), m_ASTDumpDeclTypes(ASTDumpDeclTypes), m_ASTDumpFormat(ASTDumpFormat) {}
};

int main(int argc, char **argv) {
  if (argc > 1) {
    std::unique_ptr<MyASTDumpAction> action = std::make_unique<MyASTDumpAction>();
    clang::tooling::runToolOnCode(std::move(action), argv[1], "input.c");
  }
}
