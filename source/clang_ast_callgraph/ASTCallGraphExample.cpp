#include <string>
#include <queue>

#include "clang/Analysis/CFG.h"
#include "clang/Analysis/CallGraph.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

/// 关于函数调用 CallGraph 的例子
/// 从测试来看，可以识别出来普通函数、成员函数、模板函数 等之间的相互调用
class ExampleASTConsumer : public clang::ASTConsumer {
public:
  ExampleASTConsumer() {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    CallGraph CG;
    CG.addToCallGraph(Context.getTranslationUnitDecl());
    // 将CallGraph信息打印到控制台
    CG.dump();
    // 生成 .dot 文件
    // CG.viewGraph();
  }
};

/// 下面的例子中，输出的信息如下：
//  --- Call graph Dump ---
//  Function: < root > calls: Dog::say Dog::Dog isSmaller test main
//  Function: main calls: test Dog::Dog Dog::say isSmaller
//  Function: test calls:
//  Function: isSmaller calls:
//  Function: Dog::Dog calls:
//  Function: Dog::say calls:
// 看起来：clang 会增加一个 <root>，这个 <root> 会调用全部的函数，或者成员函数，都给记录下来
int main(int argc, char **argv) {
  const std::string Code = R"CODE(
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

int main() {
    int a = 4;
    int b = 5;
    int c = test(a, b);

    Dog dog;
    dog.say();

    bool ret = isSmaller<double>(5.5f, 6.5);

    return c;
}
)CODE";

  std::vector<std::string> Args;
  Args.emplace_back("-xc++");
  std::unique_ptr<ASTUnit> Unit = tooling::buildASTFromCodeWithArgs(Code, Args, "test.cpp");

  // 执行上面的 ASTConsumer 操作
  std::unique_ptr<ExampleASTConsumer> Consumer = std::make_unique<ExampleASTConsumer>();
  Consumer->HandleTranslationUnit(Unit->getASTContext());
}
