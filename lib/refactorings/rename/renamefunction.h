#ifndef LIB_RENAMEFUNCTION_H_
#define LIB_RENAMEFUNCTION_H_

#include <llvm/ADT/StringRef.h>
#include <memory>
#include <string>

#include "../../refactoring.h"
#include "../../refactoringargs.h"
#include "../../refactoringcontext.h"

namespace Refactor {

class RenameFunction : public Refactoring
{
public:
  RenameFunction(llvm::StringRef name, const RefactoringArgs& args);

  std::unique_ptr<RefactoringAction> createAction (
    llvm::StringRef name,
    RefactoringContext& context
  ) const override;

  /// \brief get name for refactoring
  llvm::StringRef getOldName() const { return old_name_; }

  /// \brief get replacement for old name
  llvm::StringRef getNewName() const { return new_name_; }

  bool allowUnsafe() const { return allow_unsafe_; }

private:
  std::string old_name_;
  std::string new_name_;
  bool allow_unsafe_ = false;
};

} // namespace Refactor

#endif /* LIB_RENAMEFUNCTION_H_ */
