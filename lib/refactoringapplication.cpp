#include "refactoringapplication.h"

#include <clang/Tooling/Core/Replacement.h>
#include <clang/Tooling/Tooling.h>
#include <memory>
#include <vector>

#include "refactoring.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

namespace Refactor {

RefactoringApplication::RefactoringApplication(
  const clang::tooling::CompilationDatabase &compilations,
  llvm::ArrayRef<std::string> source_paths
)
: tool_(compilations, source_paths)
{
}

RefactoringApplication::~RefactoringApplication()
{
}

void RefactoringApplication::addRefactoring(const Refactoring& task)
{
  Replacements& replacements = tool_.getReplacements();
  actions_.emplace_back(task.createAction(replacements, finder_));
}

int RefactoringApplication::runAndSave()
{
  return tool_.runAndSave(newFrontendActionFactory(&finder_).get());
}

} // namespace Refactor
