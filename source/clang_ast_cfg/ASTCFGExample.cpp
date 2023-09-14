#include <iostream>
#include <string>
#include <queue>

#include "clang/Analysis/CFG.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

/// 生成 FunctionDecl 和 CXXMethodDecl 两种类型的 CFG，并进行打印。
/// CFG 是表示 AST 层面的 CFG，CFG 的组成是 CFGBlock，CFGBlock 由 CFGElement 组成
class ExampleASTVisitor
    : public RecursiveASTVisitor<ExampleASTVisitor> {
  typedef RecursiveASTVisitor<ExampleASTVisitor> base;

public:
  ExampleASTVisitor() {}

  bool TraverseDecl(Decl *D) {
    if (auto FD = llvm::dyn_cast_or_null<FunctionDecl>(D)) {
      CFG::BuildOptions BO;
      std::unique_ptr<CFG> cfg = CFG::buildCFG(FD, FD->getBody(), &(FD->getASTContext()), BO);

      // CFG 遍历，基于 worklist 的遍历，一般来说，如果是一棵树，可以认为是广度优先搜索
      // CFG 遍历，遍历的节点是 CFGBlock
      CFGBlock entry = cfg->getEntry();
      std::queue<CFGBlock> worklist;
      worklist.push(entry);

      std::set<unsigned int> visited;
      while (!worklist.empty()) {
        CFGBlock block = worklist.front();
        worklist.pop();

        if (visited.find(block.getBlockID()) != visited.end()) {
          continue ;
        }

        visited.insert(block.getBlockID());
        llvm::outs() << "visited: " << block.getBlockID() << "\n";

        // CFGBlock 内容遍历，遍历的是 CFGElement
        for (auto & it : block) {
          llvm::outs() << "==> cfg element: ";
          it.dump();
        }

        for (auto it = block.succ_begin(); it != block.succ_end(); ++it) {
          if (visited.find((*it)->getBlockID()) != visited.end()) {
            continue ;
          }

          worklist.push(**it);
        }
      }

      // 打印 CFG，生成到控制台
      LangOptions LO;
      cfg->dump(LO, false);
    } else if (auto MD = llvm::dyn_cast_or_null<CXXMethodDecl>(D)) {
      CFG::BuildOptions BO;
      std::unique_ptr<CFG> cfg = CFG::buildCFG(FD, MD->getBody(), &(FD->getASTContext()), BO);

      LangOptions LO;
      cfg->dump(LO, false);
    }
    return base::TraverseDecl(D);
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


/// 对于例子中的代码片段，打印的 CFG 内容：
//  [B5 (ENTRY)]
//   Succs (1): B4
//
// [B1]
//   1: result (ImplicitCastExpr, LValueToRValue, int)
//   2: return [B1.1];
//   Preds (2): B2 B3
//   Succs (1): B0
//
// [B2]
//   1: result = y - x
//   Preds (1): B4
//   Succs (1): B1
//
// [B3]
//   1: result = x - y
//   Preds (1): B4
//   Succs (1): B1
//
// [B4]
//   1: int result = 0;
//   2: x > y
//   T: if [B4.2]
//   Preds (1): B5
//   Succs (2): B3 B2
//
// [B0 (EXIT)]
//   Preds (1): B1
int main(int argc, char **argv) {
  const std::string Code = R"CODE(
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
  Args.emplace_back("-xc");
  std::unique_ptr<ASTUnit> Unit = tooling::buildASTFromCodeWithArgs(Code, Args, "test.c");

  // 执行上面的 ASTConsumer 操作
  std::unique_ptr<ExampleASTConsumer> Consumer = std::make_unique<ExampleASTConsumer>();
  Consumer->HandleTranslationUnit(Unit->getASTContext());
}
