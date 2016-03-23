#ifndef LIB_REFACTORINGAPPLICATION_H_
#define LIB_REFACTORINGAPPLICATION_H_

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/ArrayRef.h>
#include <string>

#include "refactoring.h"
#include "replacements.h"

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

  Replacements& getReplacements();

  int run();
  bool save();

private:
  clang::tooling::ClangTool tool_;
  clang::ast_matchers::MatchFinder finder_;

  // storage of actions
  RefactoringActions actions_;

  Replacements replacements_;
};

} // namespace Refactor

#endif /* LIB_REFACTORINGAPPLICATION_H_ */
