#include "utils.h"

#include <clang/AST/ASTContext.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceLocation.h>

using namespace clang;
using namespace llvm;

namespace Refactor {

void printRenaming(
  clang::ASTContext* context,
  clang::CharSourceRange range,
  llvm::StringRef replacement)
{
  auto& diag = context->getDiagnostics();
  static unsigned diagID = diag.getCustomDiagID(
    DiagnosticsEngine::Warning,
    "Will be replace with %0.");
  {
    auto builder = diag.Report(range.getBegin(), diagID);
    builder.AddString(replacement);
    builder.AddSourceRange(range);
  }
}


} // namespace Refactor
