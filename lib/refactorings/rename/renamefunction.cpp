#include "renamefunction.h"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/VariadicFunction.h>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

#include "../../core/parsers.h"
#include "../../core/utils.h"

using namespace clang;

namespace Refactor {

namespace {

class CallRenamer: public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
  CallRenamer(Replacements &Replace, const RenameFunction& refactoring)
    : replacements_(Replace), new_name_(refactoring.getNewName())
  {
  }

  void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override
  {
    const DeclRefExpr *M = Result.Nodes.getNodeAs<DeclRefExpr>("function");
    assert(M);

    CharSourceRange char_range = CharSourceRange::getTokenRange(SourceRange(M->getLocation()));

    printRenaming(Result.Context, char_range, new_name_);

    replacements_.addReplacement(
      Certainty::Safe,
      *Result.SourceManager,
      char_range,
      new_name_);
  }

private:
  Replacements& replacements_;
  std::string new_name_;
};

class DeclRenamer: public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
  DeclRenamer(Replacements &Replace, const RenameFunction& refactoring)
    : replacements_(Replace), new_name_(refactoring.getNewName())
  {
  }

  void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override
  {
    const FunctionDecl *D = Result.Nodes.getDeclAs<FunctionDecl>("function");
    assert(D);

    CharSourceRange char_range =
      CharSourceRange::getTokenRange(SourceRange(D->getLocation()));

    //printRenaming(Result.Context, char_range, new_name_);

    replacements_.addReplacement(
      Certainty::Safe,
      *Result.SourceManager,
      char_range,
      new_name_);
  }

private:
  Replacements& replacements_;
  std::string new_name_;
};

class RenameFunctionAction : public RefactoringAction
{
public:
  RenameFunctionAction(
    Replacements& replacements,
    clang::ast_matchers::MatchFinder& Finder,
    const RenameFunction& refactoring)
    :
      call_renamer_(replacements, refactoring),
      decl_renamer_(replacements, refactoring)
  {
    using namespace clang;
    using namespace clang::ast_matchers;

    if (refactoring.allowUnsafe())
    {
      Finder.addMatcher(
        declRefExpr(
            to(functionDecl(hasName(refactoring.getOldName()))),

            // don't replace if used in function pointer template parameter substitutions
            unless(hasAncestor(substNonTypeTemplateParmExpr()))
        ).bind("function"),
        &call_renamer_);
    }
    else
    {
      Finder.addMatcher(
        declRefExpr(
            to(functionDecl(hasName(refactoring.getOldName()))),
            unless(isInTemplateInstantiation())
        ).bind("function"),
        &call_renamer_);
    }

    Finder.addMatcher(
      functionDecl(hasName(refactoring.getOldName())).bind("function"),
      &decl_renamer_);
  }

private:
  CallRenamer call_renamer_;
  DeclRenamer decl_renamer_;
};


}  // namespace

// RenameFunction

RenameFunction::~RenameFunction()
{
}

std::unique_ptr<RenameFunction> RenameFunction::createFromCommand(llvm::StringRef command)
{
  // parse commands and check argument count
  CommandOptions options = Parsers::parseCommandOptions(command);
  if (options.arguments.size() != 2)
  {
    // TODO:
    throw std::invalid_argument(
      "-rename-function: Failed to parse parameters.\n"
      "Syntax: -rename-function OLD,NEW");
  }

  // create refactoring
  return createFromNames(options.arguments[0], options.arguments[1]);
}

std::unique_ptr<RenameFunction> RenameFunction::createFromNames(
  llvm::StringRef definition, llvm::StringRef new_name)
{
  std::string definition_s;

  // ensure that `definition` starts with `::`
  if (!definition.startswith("::"))
  {
    definition_s.assign("::", 2);
    definition_s.append(definition.begin(), definition.end());
  }
  else
  {
    definition_s = definition;
  }

  // parse `definition`
  //FuncDef decl = Parsers::parseFunctionDefinition(definition);

  // create refactoring
  auto result = std::unique_ptr<RenameFunction>(new RenameFunction(
    definition_s, new_name // TODO: check input
  ));
  return result;
}

RenameFunction::RenameFunction(std::string definition, std::string new_name)
: old_name_(std::move(definition)), new_name_(std::move(new_name))
{
}

std::unique_ptr<RefactoringAction> RenameFunction::createAction(
  Replacements& replacements,
  clang::ast_matchers::MatchFinder& Finder) const
{
  return std::make_unique<RenameFunctionAction>(replacements, Finder, *this);
}

} // namespace Refactor
