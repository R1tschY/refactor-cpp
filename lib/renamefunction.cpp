#include "renamefunction.h"

#include <boost/algorithm/string/predicate.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <llvm-3.6/llvm/ADT/VariadicFunction.h>
#include <cassert>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include "core/parsers.h"
#include "core/utils.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

namespace Refactor {

namespace {

class CallRenamer: public MatchFinder::MatchCallback
{
public:
  CallRenamer(Replacements &Replace, const RenameFunction& refactoring)
    : replacements_(&Replace), new_name_(refactoring.getNewName())
  {
  }

  virtual void run(const MatchFinder::MatchResult &Result)
  {
    const DeclRefExpr *M = Result.Nodes.getNodeAs<DeclRefExpr>("function");
    assert(M);

    CharSourceRange char_range = CharSourceRange::getTokenRange(SourceRange(M->getLocation()));

    printRenaming(Result.Context, char_range, new_name_);

    replacements_->insert(
      Replacement(
        *Result.SourceManager,
        char_range,
        new_name_));
  }

private:
  Replacements *replacements_;
  std::string new_name_;
};

class DeclRenamer: public MatchFinder::MatchCallback
{
public:
  DeclRenamer(Replacements &Replace, const RenameFunction& refactoring)
    : replacements_(&Replace), new_name_(refactoring.getNewName())
  {
  }

  virtual void run(const MatchFinder::MatchResult &Result)
  {
    const FunctionDecl *D = Result.Nodes.getDeclAs<FunctionDecl>("function");
    assert(D);

    CharSourceRange char_range =
      CharSourceRange::getTokenRange(SourceRange(D->getLocation()));

    //printRenaming(Result.Context, char_range, new_name_);

    replacements_->insert(
      Replacement(
        *Result.SourceManager,
        char_range,
        new_name_));
  }

private:
  Replacements *replacements_;
  std::string new_name_;
};

class RenameFunctionAction : public RefactoringAction
{
public:
  RenameFunctionAction(
    clang::tooling::Replacements& replacements,
    clang::ast_matchers::MatchFinder& Finder,
    const RenameFunction& refactoring)
    :
      call_renamer_(replacements, refactoring),
      decl_renamer_(replacements, refactoring)
  {
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

std::unique_ptr<RenameFunction> RenameFunction::createFromCommand(std::string command)
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
  std::string definition, std::string new_name)
{
  // ensure that `definition` starts with `::`
  if (!boost::algorithm::starts_with(definition, "::"))
  {
    const char* double_colon = "::";
    definition.insert(definition.begin(), double_colon, double_colon + 2);
  }

  // parse `definition`
  //FuncDef decl = Parsers::parseFunctionDefinition(definition);

  // create refactoring
  auto result = std::unique_ptr<RenameFunction>(new RenameFunction(
    definition, new_name // TODO: check input
  ));
  return result;
}

RenameFunction::RenameFunction(std::string definition, std::string new_name)
: old_name_(definition), new_name_(new_name)
{
}

std::unique_ptr<RefactoringAction> RenameFunction::createAction(
  clang::tooling::Replacements& replacements,
  clang::ast_matchers::MatchFinder& Finder) const
{
  return std::make_unique<RenameFunctionAction>(replacements, Finder, *this);
}

} // namespace Refactor
