#ifndef LIB_REFACTORINGTASK_H_
#define LIB_REFACTORINGTASK_H_

#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/ArrayRef.h>
#include <string>

#include "refactoring.h"
#include "refactoringcontext.h"
#include "replacementgroups.h"
#include "utils/shortcuts.h"
#include "internals/clang.h"

namespace Refactor {

class Clang;

/// \brief
class RefactoringTask
{
public:
  RefactoringTask(
    Clang& clang,
    const CompilationDatabase &compilations,
    ArrayRef<std::string> source_paths);

  void addRefactoring(const Refactoring& task);

  ReplacementGroups& getReplacementGroups();

  int run();
  bool save();

private:
  Clang& clang_;

  clang::tooling::ClangTool tool_;

  // storage of actions
  RefactoringActions actions_;

  RefactoringContext context_;
};

} // namespace Refactor

#endif /* LIB_REFACTORINGTASK_H_ */
