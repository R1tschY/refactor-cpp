#include "refactoringapplication.h"

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Core/Replacement.h>
#include <clang/Tooling/Tooling.h>
#include <llvm-3.6/llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm-3.6/llvm/Support/raw_ostream.h>
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
  Replacements& replacements = tool_.getReplacements();
  actions_.emplace_back(task.createAction(replacements, finder_));
}

int RefactoringApplication::run()
{
  return tool_.run(newFrontendActionFactory(&finder_).get());
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

  if (!tool_.applyAllReplacements(rewriter)) {
    llvm::errs() << "Skipped some replacements.\n";
  }

  return !rewriter.overwriteChangedFiles();
}

Replacements& RefactoringApplication::getReplacements()
{
  return tool_.getReplacements();
}

} // namespace Refactor
