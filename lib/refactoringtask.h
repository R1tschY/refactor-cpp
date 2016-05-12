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
#include "3th-party/Tooling.h"

namespace Refactor {

/// \brief
class RefactoringTask
{
public:
  RefactoringTask(
    const CompilationDatabase &compilations,
    ArrayRef<std::string> source_paths);

  ~RefactoringTask();

  void addRefactoring(const Refactoring& task);

  ReplacementGroups& getReplacementGroups();

  int run();
  bool save();

private:
  clang::tooling::ClangTool_ tool_;

  // storage of actions
  RefactoringActions actions_;

  RefactoringContext context_;
};

} // namespace Refactor

#endif /* LIB_REFACTORINGTASK_H_ */
