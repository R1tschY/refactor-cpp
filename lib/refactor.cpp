#include "refactor.h"

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Registry.h>
#include <llvm/Support/Signals.h>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "refactoring.h"
#include "refactoringargs.h"
#include "refactoringmodule.h"
#include "refactoringregistry.h"
#include "refactoringtask.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

template class llvm::Registry<Refactor::RefactoringModuleRegistry>;

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

//
// Refactor C++ Options
//
static cl::OptionCategory RefactorCppCategory("Refactor C++ Options");

static cl::opt<bool> FinalSyntaxCheck(
  "final-syntax-check",
  cl::desc("Check for correct syntax after applying refactoring"),
  cl::init(false),
  cl::cat(RefactorCppCategory));

//
// Refactor C++ Refactorings
//

static cl::opt<std::string> RenameOption(
  "rename-function",
  cl::desc("Rename a function"),
  cl::ValueRequired,
  cl::Optional,
  cl::cat(RefactorCppCategory));

namespace Refactor {

int main(int argc, const char *argv[])
{
  llvm::sys::PrintStackTraceOnErrorSignal();

  CommonOptionsParser OptionsParser(argc, argv, RefactorCppCategory);
  RefactoringTask app(
    OptionsParser.getCompilations(),
    OptionsParser.getSourcePathList());

  Refactorings refactorings;

  // get all refactoring factories
  RefactoringFactories factories;
  for (
    auto i = RefactoringModuleRegistry::begin();
    i != RefactoringModuleRegistry::end();
    ++i)
  {
    i->instantiate()->addFactories(factories);
  }

  try
  {
    if (RenameOption.getNumOccurrences() > 0)
    {
      refactorings.emplace_back(
        factories.create(
          "rename-function",
          RefactoringArgs::fromString(RenameOption.getValue()))
      );
    }
    else
    {
      llvm::errs() << "no refactorings given\n";
      return 1;
    }
  }
  catch(const std::exception& e)
  {
    llvm::errs() << "Error while parsing command arguments: " << e.what() << '\n';
    return 1;
  }

  try
  {
    for (const auto& refactoring : refactorings)
      app.addRefactoring(*refactoring);
  }
  catch(const std::exception& e)
  {
    llvm::errs() << "Error while creating refactorings: " << e.what() << '\n';
    return 1;
  }

  int returncode = app.run();
  if (returncode != 0)
  {
    return returncode;
  }

  return app.save() ? 0 : 1;
}

} // namespace Refactor
