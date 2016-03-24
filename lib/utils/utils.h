#ifndef LIB_UTILS_UTILS_H_
#define LIB_UTILS_UTILS_H_

#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceLocation.h>

namespace Refactor {

void printRenaming(clang::ASTContext* context, clang::CharSourceRange range, llvm::StringRef replacement);

} // namespace Refactor

#endif /* LIB_UTILS_UTILS_H_ */
