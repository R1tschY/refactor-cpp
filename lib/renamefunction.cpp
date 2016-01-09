#include "renamefunction.h"

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
#include <boost/algorithm/string/predicate.hpp>
#include <cassert>
#include <iostream>
#include <set>
#include <utility>
#include <regex>

#include "core/parsers.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

namespace Refactor {

namespace {

std::string decl2str(const clang::Decl *d, SourceManager &sm) {
    clang::SourceLocation b(d->getLocStart()), _e(d->getLocEnd());
    clang::SourceLocation e(clang::Lexer::getLocForEndOfToken(_e, 0, sm, LangOptions()));
    return std::string(sm.getCharacterData(b),
        sm.getCharacterData(e)-sm.getCharacterData(b));
}

std::string expr2str(const clang::Expr *d, SourceManager &sm) {
    clang::SourceLocation b(d->getLocStart()), _e(d->getLocEnd());
    clang::SourceLocation e(clang::Lexer::getLocForEndOfToken(_e, 0, sm, LangOptions()));
    return std::string(sm.getCharacterData(b),
        sm.getCharacterData(e)-sm.getCharacterData(b));
}

class CallRenamer: public MatchFinder::MatchCallback
{
public:
  CallRenamer(Replacements &Replace, const RenameFunction& refactoring)
    : replacements_(&Replace), new_name_(refactoring.getNewName())
  {
  }

  virtual void run(const MatchFinder::MatchResult &Result)
  {
    const CallExpr *M = Result.Nodes.getNodeAs<CallExpr>("function");
    assert(M);

    std::cout << "Renaming: " << expr2str(M, *Result.SourceManager) << '\n';

    replacements_->insert(
      Replacement(
        *Result.SourceManager,
        CharSourceRange::getTokenRange(SourceRange(M->getExprLoc())),
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

    std::cout << "Renaming: " << decl2str(D, *Result.SourceManager) << '\n';

    replacements_->insert(
      Replacement(
        *Result.SourceManager,
        CharSourceRange::getTokenRange(SourceRange(D->getLocation())),
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
    Finder.addMatcher(
      callExpr(callee(functionDecl(hasName(refactoring.getOldName())))).bind(
        "function"),
      &call_renamer_);

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
