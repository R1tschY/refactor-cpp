#include "renamefunction.h"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/VariadicFunction.h>
#include <cassert>
#include <stdexcept>
#include <vector>

#include "../../replacementbuilder.h"
#include "../../replacementgroups.h"

using namespace clang;
using namespace clang::ast_matchers;


namespace Refactor {

namespace {

class RenameFunctionAction : public RefactoringAction
{
  class CallRenamer: public MatchFinder::MatchCallback {
  public:
    CallRenamer(RenameFunctionAction& action)
      : action_(action)
    { }

    void run(const MatchFinder::MatchResult &Result) override
    {
      const DeclRefExpr *M = Result.Nodes.getNodeAs<DeclRefExpr>("function");
      assert(M);

      auto builder = action_.refactor(
        Confidence::Safe, "rename function", *Result.SourceManager
      );
      builder.replace(
        CharSourceRange::getTokenRange(SourceRange(M->getLocation())),
        action_.refactoring_.getNewName()
      );
    }

  private:
    RenameFunctionAction& action_;
  };

  class DeclRenamer: public MatchFinder::MatchCallback {
  public:
    DeclRenamer(RenameFunctionAction& action)
      : action_(action)
    { }

    void run(const MatchFinder::MatchResult &Result) override
    {
      const FunctionDecl *D = Result.Nodes.getDeclAs<FunctionDecl>("function");
      assert(D);

      auto builder = action_.refactor(
        Confidence::Safe, "rename function", *Result.SourceManager
      );
      builder.replace(
        CharSourceRange::getTokenRange(SourceRange(D->getLocation())),
        action_.refactoring_.getNewName()
      );
    }

  private:
    RenameFunctionAction& action_;
  };


public:
  RenameFunctionAction(
    llvm::StringRef name,
    RefactoringContext& context,
    const RenameFunction& refactoring)
    : RefactoringAction(name, context),
      call_renamer_(*this),
      decl_renamer_(*this),
      refactoring_(refactoring)
  { }

  void registerMatchers(clang::ast_matchers::MatchFinder& finder) override
  {
    using namespace clang;
    using namespace clang::ast_matchers;

    if (refactoring_.allowUnsafe())
    {
      finder.addMatcher(
        declRefExpr(
            to(functionDecl(hasName(refactoring_.getOldName()))),

            // don't replace if used in function pointer template parameter substitutions
            unless(hasAncestor(substNonTypeTemplateParmExpr()))
        ).bind("function"),
        &call_renamer_);
    }
    else
    {
      finder.addMatcher(
        declRefExpr(
            to(functionDecl(hasName(refactoring_.getOldName()))),
            unless(isInTemplateInstantiation())
        ).bind("function"),
        &call_renamer_);
    }

    finder.addMatcher(
      functionDecl(hasName(refactoring_.getOldName())).bind("function"),
      &decl_renamer_);
  }

private:
  CallRenamer call_renamer_;
  DeclRenamer decl_renamer_;
  const RenameFunction& refactoring_;
};

}  // namespace

// RenameFunction

RenameFunction::RenameFunction(llvm::StringRef name, const RefactoringArgs& allargs)
: Refactoring(name)
{
  auto& args = allargs.getArgs();
  if (args.size() != 2)
  {
    // TODO:
    throw std::invalid_argument(
      "-rename-function: Failed to parse parameters.\n"
      "Syntax: -rename-function OLD,NEW");
  }

  // ensure that `definition` starts with `::`
  llvm::StringRef definition = args[0];
  if (!definition.startswith("::"))
  {
    old_name_.reserve(definition.size() + 2);
    old_name_.assign("::", 2);
    old_name_.append(definition.begin(), definition.end());
  }
  else
  {
    old_name_ = definition;
  }

  new_name_ = args[1];

  // TODO: parse `definition`
  //FuncDef decl = Parsers::parseFunctionDefinition(definition);
}

std::unique_ptr<RefactoringAction> RenameFunction::createAction(
  llvm::StringRef name,
  RefactoringContext& context
) const
{
  return std::make_unique<RenameFunctionAction>(name, context, *this);
}

} // namespace Refactor
