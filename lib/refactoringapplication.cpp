#include "refactoringapplication.h"

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Core/Replacement.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <vector>

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
  actions_.emplace_back(task.createAction(replacements_, finder_));
}

int RefactoringApplication::run()
{
  int result = tool_.run(newFrontendActionFactory(&finder_).get());
  replacements_.deduplicate();
  // TODO: search conflicts
  return result;
}

bool RefactoringApplication::save()
{
  LangOptions default_lang_options;
  IntrusiveRefCntPtr<DiagnosticOptions> diag_opts = new DiagnosticOptions();
  TextDiagnosticPrinter diag_printer(llvm::errs(), &*diag_opts);
  DiagnosticsEngine diagnostics(
    IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs()),
    &*diag_opts,
    &diag_printer,
    false);
  SourceManager sources(diagnostics, tool_.getFiles());
  Rewriter rewriter(sources, default_lang_options);

  auto replacements = replacements_.getRawReplacements();
  if (!clang::tooling::applyAllReplacements(replacements, rewriter))
  {
    llvm::errs() << "Skipped some replacements.\n";
  }

  return !rewriter.overwriteChangedFiles();
}

Replacements& RefactoringApplication::getReplacements()
{
  return replacements_;
}

} // namespace Refactor
