#ifndef LIB_REFACTORING_H_
#define LIB_REFACTORING_H_

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <memory>
#include <vector>

#include "replacements.h"

namespace Refactor {

class RefactoringAction
{
public:
  virtual ~RefactoringAction() = default;
};

/// \brief
class Refactoring
{
public:
  virtual ~Refactoring() = default;

  virtual std::unique_ptr<RefactoringAction> createAction
  (
    Replacements& replacements,
    clang::ast_matchers::MatchFinder& Finder
  ) const = 0;
};

using Refactorings = std::vector<std::unique_ptr<Refactoring>>;

using RefactoringActions = std::vector<std::unique_ptr<RefactoringAction>>;

} // namespace Refactor

#endif /* LIB_REFACTORING_H_ */

