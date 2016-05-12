//===--- Tooling.h - Framework for standalone Clang tools -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file implements functions to run clang tools standalone instead
//  of running them as a plugin.
//
//  A ClangTool is initialized with a CompilationDatabase and a set of files
//  to run over. The tool will then run a user-specified FrontendAction over
//  all TUs in which the given files are compiled.
//
//  It is also possible to run a FrontendAction over a snippet of code by
//  calling runToolOnCode, which is useful for unit testing.
//
//  Applications that need more fine grained control over how to run
//  multiple FrontendActions over code can use ToolInvocation.
//
//  Example tools:
//  - running clang -fsyntax-only over source code from an editor to get
//    fast syntax checks
//  - running match/replace tools over C++ code
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLING_TOOLING_H_PATCHED
#define LLVM_CLANG_TOOLING_TOOLING_H_PATCHED

#include <clang/Frontend/ASTUnit.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/ADT/StringRef.h>
#include <utility>

namespace clang {

namespace driver {
class Compilation;
} // end namespace driver

class CompilerInvocation;
class SourceManager;
class FrontendAction;

namespace tooling {

/// \brief Utility to run a FrontendAction over a set of files.
///
/// This class is written to be usable for command line utilities.
/// By default the class uses ClangSyntaxOnlyAdjuster to modify
/// command line arguments before the arguments are used to run
/// a frontend action. One could install an additional command line
/// arguments adjuster by calling the appendArgumentsAdjuster() method.
class ClangTool_ {
 public:
  /// \brief Constructs a clang tool to run over a list of files.
  ///
  /// \param Compilations The CompilationDatabase which contains the compile
  ///        command lines for the given source paths.
  /// \param SourcePaths The source files to run over. If a source files is
  ///        not found in Compilations, it is skipped.
  /// \param PCHContainerOps The PCHContainerOperations for loading and creating
  /// clang modules.
  ClangTool_(const CompilationDatabase &Compilations,
            ArrayRef<std::string> SourcePaths,
            std::shared_ptr<PCHContainerOperations> PCHContainerOps =
                std::make_shared<PCHContainerOperations>());

  ~ClangTool_();

  /// \brief Set a \c DiagnosticConsumer to use during parsing.
  void setDiagnosticConsumer(DiagnosticConsumer *DiagConsumer) {
    this->DiagConsumer = DiagConsumer;
  }

  /// \brief Map a virtual file to be used while running the tool.
  ///
  /// \param FilePath The path at which the content will be mapped.
  /// \param Content A null terminated buffer of the file's content.
  void mapVirtualFile(StringRef FilePath, StringRef Content);

  /// \brief Append a command line arguments adjuster to the adjuster chain.
  ///
  /// \param Adjuster An argument adjuster, which will be run on the output of
  ///        previous argument adjusters.
  void appendArgumentsAdjuster(ArgumentsAdjuster Adjuster);

  /// \brief Clear the command line arguments adjuster chain.
  void clearArgumentsAdjusters();

  /// Runs an action over all files specified in the command line.
  ///
  /// \param Action Tool action.
  int run(ToolAction *Action);

  /// \brief Create an AST for each file specified in the command line and
  /// append them to ASTs.
  int buildASTs(std::vector<std::unique_ptr<ASTUnit>> &ASTs);

  /// \brief Returns the file manager used in the tool.
  ///
  /// The file manager is shared between all translation units.
  FileManager &getFiles() { return *Files; }

 private:
  const CompilationDatabase &Compilations;
  std::vector<std::string> SourcePaths;
  std::shared_ptr<PCHContainerOperations> PCHContainerOps;

  llvm::IntrusiveRefCntPtr<FileManager> Files;
  // Contains a list of pairs (<file name>, <file content>).
  std::vector< std::pair<StringRef, StringRef> > MappedFileContents;

  ArgumentsAdjuster ArgsAdjuster;

  DiagnosticConsumer *DiagConsumer;
};

} // end namespace tooling
} // end namespace clang

#endif // LLVM_CLANG_TOOLING_TOOLING_H_PATCHED
