#ifndef LIB_REFACTORINGAPPLICATION_H_
#define LIB_REFACTORINGAPPLICATION_H_

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Refactoring.h>
#include <llvm-3.6/llvm/ADT/ArrayRef.h>
#include <string>

#include "refactoring.h"

namespace Refactor {

/// \brief
class RefactoringApplication
{
public:
  RefactoringApplication(
    const clang::tooling::CompilationDatabase &compilations,
    llvm::ArrayRef<std::string> source_paths);

  ~RefactoringApplication();

  void addRefactoring(const Refactoring& task);
  int runAndSave();

private:
  clang::tooling::RefactoringTool tool_;

  clang::ast_matchers::MatchFinder finder_;

  // storage of actions
  RefactoringActions actions_;
};

} // namespace Refactor

#endif /* LIB_REFACTORINGAPPLICATION_H_ */
