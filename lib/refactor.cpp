#include "refactor.h"

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm-3.6/llvm/Support/CommandLine.h>
#include <llvm-3.6/llvm/Support/raw_ostream.h>
#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "refactoring.h"
#include "refactoringapplication.h"
#include "renamefunction.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

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
  CommonOptionsParser OptionsParser(argc, argv, RefactorCppCategory);
  RefactoringApplication app(
    OptionsParser.getCompilations(),
    OptionsParser.getSourcePathList());

  Refactorings refactorings;

  try
  {
    if (RenameOption.getNumOccurrences() > 0)
    {
      refactorings.emplace_back(
        RenameFunction::createFromCommand(RenameOption.getValue())
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

  llvm::outs() << "Replacements collected:\n";
  for (auto &r : app.getReplacements()) {
    llvm::outs() << r.toString() << "\n";
  }

  return app.save() ? 0 : 1;
}

} // namespace Refactor
