#include "refactoringtask.h"

#include <clang/ASTMatchers/ASTMatchFinder.h>
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

#include "utils/algorithm.h"
#include "internals/clang.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

namespace Refactor {

namespace {

ArgumentsAdjuster getClangResourcePathAdjuster(const Clang& clang)
{
#if REFACTOR_LLVM_PREQE(3,8,0)
  return [=](const CommandLineArguments& in, StringRef Filename)
#else
  return [=](const CommandLineArguments& in)
#endif
  {
    CommandLineArguments result = in;

    // allow user defined resource dir
    for (StringRef arg : result)
      if (arg.startswith("-resource-dir"))
        return result;

    result.push_back("-resource-dir=" + clang.getResourceDir());
    return result;
  };
}

} // namespace

RefactoringTask::RefactoringTask(
  Clang& clang,
  const clang::tooling::CompilationDatabase &compilations,
  llvm::ArrayRef<std::string> source_paths
)
: clang_(clang),
  tool_(compilations, source_paths)
{
  tool_.appendArgumentsAdjuster(getClangResourcePathAdjuster(clang));
}

void RefactoringTask::addRefactoring(const Refactoring& task)
{
  actions_.push_back(task.createAction(context_));
}

int RefactoringTask::run()
{
  // get matchers
  clang::ast_matchers::MatchFinder finder;
  for (auto& action : actions_)
  {
    action->registerMatchers(finder);
  }

  // run matchers and generate replacements
  int result = tool_.run(newFrontendActionFactory(&finder).get());
  // TODO: search conflicts
  return result;
}

bool RefactoringTask::save()
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

  auto replacements = getReplacements(context_.getReplacements());
  Support::deduplicate(replacements);

  for (auto& replacement : replacements)
  {
    llvm::outs() << replacement.toString() << '\n';
  }

  if (!clang::tooling::applyAllReplacements(replacements, rewriter))
  {
    llvm::errs() << "Skipped some replacements.\n";
  }

  return !rewriter.overwriteChangedFiles();
}

ReplacementGroups& RefactoringTask::getReplacementGroups()
{
  return context_.getReplacements();
}

} // namespace Refactor
