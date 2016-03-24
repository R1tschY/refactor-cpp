#ifndef LIB_REFACTORING_H_
#define LIB_REFACTORING_H_

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <llvm/ADT/StringRef.h>
#include <memory>
#include <string>
#include <vector>

#include "refactoringcontext.h"
#include "replacementbuilder.h"

namespace Refactor {

/// \brief a concrete refactoring
///
/// e.x. find occurrences of class A and replace A with B
class RefactoringAction
{
public:
  explicit RefactoringAction(llvm::StringRef name, RefactoringContext& context)
  : name_(name), context_(context)
  { }

  virtual ~RefactoringAction() = default;

  /// \brief add matchers to \p finder to find nodes in ast
  virtual void registerMatchers(clang::ast_matchers::MatchFinder& finder) {}

  llvm::StringRef getName() const { return name_; }

protected:
  ReplacementBuilder refactor(
    Confidence confidence,
    llvm::StringRef description,
    const clang::SourceManager& sources);

private:
  std::string name_;
  RefactoringContext& context_;
};

/// \brief a abstract refactoring
///
/// e.x. rename class A to B
class Refactoring
{
public:
  Refactoring(llvm::StringRef name)
  : name_(name)
  { }

  virtual ~Refactoring() = default;

  std::unique_ptr<RefactoringAction> createAction
  (
    RefactoringContext& context
  ) const;

private:
  std::string name_;

  virtual std::unique_ptr<RefactoringAction> createAction
  (
    llvm::StringRef name,
    RefactoringContext& context
  ) const = 0;
};

using Refactorings = std::vector<std::unique_ptr<Refactoring>>;

using RefactoringActions = std::vector<std::unique_ptr<RefactoringAction>>;

} // namespace Refactor

#endif /* LIB_REFACTORING_H_ */

