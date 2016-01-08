#ifndef LIB_RENAMEFUNCTION_H_
#define LIB_RENAMEFUNCTION_H_

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/Core/Replacement.h>
#include <memory>
#include <string>

#include "refactoring.h"

namespace Refactor {

class RenameFunction : public Refactoring
{
public:
  RenameFunction(std::string old_name, std::string new_name);
  virtual ~RenameFunction();

  std::unique_ptr<RefactoringAction> createAction (
    clang::tooling::Replacements& replacements,
    clang::ast_matchers::MatchFinder& Finder
  ) const override;

  const std::string& getOldName() const { return old_name_; }
  const std::string& getNewName() const { return new_name_; }

private:
  std::string old_name_;
  std::string new_name_;
};

} // namespace Refactor

#endif /* LIB_RENAMEFUNCTION_H_ */