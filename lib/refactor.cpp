#include "refactor.h"

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm-3.6/llvm/Support/CommandLine.h>
#include <memory>
#include <vector>

#include "refactoring.h"
#include "refactoringapplication.h"
#include "renamefunction.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");

namespace Refactor {

int main(int argc, const char *argv[])
{
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  RefactoringApplication app(
    OptionsParser.getCompilations(),
    OptionsParser.getSourcePathList());

  Refactorings refactorings;
  refactorings.emplace_back(
    std::make_unique<RenameFunction>("::func", "function")
  );

  for (const auto& refactoring : refactorings)
    app.addRefactoring(*refactoring);

  return app.runAndSave();
}

} // namespace Refactor
