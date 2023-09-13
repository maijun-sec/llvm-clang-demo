#include <string>
#include <iostream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/AST/ASTDumperUtils.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

int main(int argc, char **argv) {
  const std::string Code = R"S(
    int main() {
        int x = 4;
        return x;
    }
)S";

  std::vector<std::string> Args;
  Args.emplace_back("-xc");
  // 通过 tooling 中的 buildASTFromCodeWithArgs 生成 AST
  // CAUTION: 虽然在 LLVM 源码中，提示 Code 是 C++ 代码，但是这里，修改上面 Args 中， -xc 和 -xc++ 后，能够区分 C 和 C++ 代码
  // 说明该函数支持同时解析 C 和 C++ 代码。
  // 但是有一个问题：貌似没有办法传递工作路径，因此 Args 中如果涉及文件路径的话，应该都需要提前处理成绝对路径
  std::unique_ptr<ASTUnit> Unit = tooling::buildASTFromCodeWithArgs(Code, Args, "test.c");

  // 打印文件名
  std::cout << Unit->getMainFileName().str() << "\n";

  // 创建 dump AST 的 ASTConsumer，执行 AST 打印
  std::unique_ptr<ASTConsumer> Dumper = CreateASTDumper(nullptr , "", true, true, true, true, ADOF_Default);
  Dumper->HandleTranslationUnit(Unit->getASTContext());
}
