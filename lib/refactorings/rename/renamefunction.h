#ifndef LIB_RENAMEFUNCTION_H_
#define LIB_RENAMEFUNCTION_H_

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <llvm/ADT/StringRef.h>
#include <memory>
#include <string>

#include "../../refactoring.h"
#include "../../replacements.h"

namespace Refactor {

class RenameFunction : public Refactoring
{
public:
  static std::unique_ptr<RenameFunction> createFromCommand(llvm::StringRef command);
  static std::unique_ptr<RenameFunction> createFromNames(llvm::StringRef old_name,
    llvm::StringRef new_name);

  virtual ~RenameFunction();

  std::unique_ptr<RefactoringAction> createAction (
    Replacements& replacements,
    clang::ast_matchers::MatchFinder& Finder
  ) const override;

  llvm::StringRef getOldName() const { return old_name_; }
  llvm::StringRef getNewName() const { return new_name_; }
  bool allowUnsafe() const { return allow_unsafe_; }

private:
  RenameFunction(std::string definition, std::string new_name);
  std::string old_name_;
  std::string new_name_;
  bool allow_unsafe_ = false;
};

} // namespace Refactor

#endif /* LIB_RENAMEFUNCTION_H_ */
