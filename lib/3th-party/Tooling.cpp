//===--- Tooling.cpp - Running clang standalone tools ---------------------===//
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
//===----------------------------------------------------------------------===//

#include "Tooling.h"

#include <clang/Basic/FileSystemOptions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <algorithm>
#include <cassert>
#include <functional>
#include <system_error>

// For chdir, see the comment in ClangTool::run for more information.
#ifdef LLVM_ON_WIN32
#  include <direct.h>
#else
#  include <unistd.h>
#endif

#define DEBUG_TYPE "clang-tooling"

namespace clang {
namespace tooling {

ClangTool_::ClangTool_(const CompilationDatabase &Compilations,
                     ArrayRef<std::string> SourcePaths,
                     std::shared_ptr<PCHContainerOperations> PCHContainerOps)
    : Compilations(Compilations), SourcePaths(SourcePaths),
      PCHContainerOps(PCHContainerOps),
      Files(new FileManager(FileSystemOptions())), DiagConsumer(nullptr) {
  appendArgumentsAdjuster(getClangStripOutputAdjuster());
  appendArgumentsAdjuster(getClangSyntaxOnlyAdjuster());
}

ClangTool_::~ClangTool_() {}

void ClangTool_::mapVirtualFile(StringRef FilePath, StringRef Content) {
  MappedFileContents.push_back(std::make_pair(FilePath, Content));
}

void ClangTool_::appendArgumentsAdjuster(ArgumentsAdjuster Adjuster) {
  if (ArgsAdjuster)
    ArgsAdjuster = combineAdjusters(ArgsAdjuster, Adjuster);
  else
    ArgsAdjuster = Adjuster;
}

void ClangTool_::clearArgumentsAdjusters() {
  ArgsAdjuster = nullptr;
}

int ClangTool_::run(ToolAction *Action) {
  // Exists solely for the purpose of lookup of the resource path.
  // This just needs to be some symbol in the binary.
  static int StaticSymbol;
  // The driver detects the builtin header path based on the path of the
  // executable.
  // FIXME: On linux, GetMainExecutable is independent of the value of the
  // first argument, thus allowing ClangTool and runToolOnCode to just
  // pass in made-up names here. Make sure this works on other platforms.
  std::string MainExecutable =
      llvm::sys::fs::getMainExecutable("clang_tool", &StaticSymbol);

  llvm::SmallString<128> InitialDirectory;
  if (std::error_code EC = llvm::sys::fs::current_path(InitialDirectory))
    llvm::report_fatal_error("Cannot detect current path: " +
                             Twine(EC.message()));
  bool ProcessingFailed = false;
  for (const auto &SourcePath : SourcePaths) {
    std::string File(getAbsolutePath(SourcePath));

    // Currently implementations of CompilationDatabase::getCompileCommands can
    // change the state of the file system (e.g.  prepare generated headers), so
    // this method needs to run right before we invoke the tool, as the next
    // file may require a different (incompatible) state of the file system.
    //
    // FIXME: Make the compilation database interface more explicit about the
    // requirements to the order of invocation of its members.
    std::vector<CompileCommand> CompileCommandsForFile =
        Compilations.getCompileCommands(File);
    if (CompileCommandsForFile.empty()) {
      // FIXME: There are two use cases here: doing a fuzzy
      // "find . -name '*.cc' |xargs tool" match, where as a user I don't care
      // about the .cc files that were not found, and the use case where I
      // specify all files I want to run over explicitly, where this should
      // be an error. We'll want to add an option for this.
      llvm::errs() << "Skipping " << File << ". Compile command not found.\n";
      continue;
    }
    for (CompileCommand &CompileCommand : CompileCommandsForFile) {
      // FIXME: chdir is thread hostile; on the other hand, creating the same
      // behavior as chdir is complex: chdir resolves the path once, thus
      // guaranteeing that all subsequent relative path operations work
      // on the same path the original chdir resulted in. This makes a
      // difference for example on network filesystems, where symlinks might be
      // switched during runtime of the tool. Fixing this depends on having a
      // file system abstraction that allows openat() style interactions.
      if (chdir(CompileCommand.Directory.c_str()))
        llvm::report_fatal_error("Cannot chdir into \"" +
                                 Twine(CompileCommand.Directory) + "\n!");
      std::vector<std::string> CommandLine = CompileCommand.CommandLine;
      assert(!CommandLine.empty());
      CommandLine[0] = MainExecutable;
      if (ArgsAdjuster)
        CommandLine = ArgsAdjuster(CommandLine);
      assert(!CommandLine.empty());

      // FIXME: We need a callback mechanism for the tool writer to output a
      // customized message for each file.
      DEBUG({ llvm::dbgs() << "Processing: " << File << ".\n"; });
      ToolInvocation Invocation(std::move(CommandLine), Action, Files.get(),
                                PCHContainerOps);
      Invocation.setDiagnosticConsumer(DiagConsumer);
      for (const auto &MappedFile : MappedFileContents)
        Invocation.mapVirtualFile(MappedFile.first, MappedFile.second);
      if (!Invocation.run()) {
        // FIXME: Diagnostics should be used instead.
        llvm::errs() << "Error while processing " << File << ".\n";
        ProcessingFailed = true;
      }
      // Return to the initial directory to correctly resolve next file by
      // relative path.
      if (chdir(InitialDirectory.c_str()))
        llvm::report_fatal_error("Cannot chdir into \"" +
                                 Twine(InitialDirectory) + "\n!");
    }
  }
  return ProcessingFailed ? 1 : 0;
}

namespace {

class ASTBuilderAction : public ToolAction {
  std::vector<std::unique_ptr<ASTUnit>> &ASTs;

public:
  ASTBuilderAction(std::vector<std::unique_ptr<ASTUnit>> &ASTs) : ASTs(ASTs) {}

  bool runInvocation(CompilerInvocation *Invocation, FileManager *Files,
                     std::shared_ptr<PCHContainerOperations> PCHContainerOps,
                     DiagnosticConsumer *DiagConsumer) override {
    // FIXME: This should use the provided FileManager.
    std::unique_ptr<ASTUnit> AST = ASTUnit::LoadFromCompilerInvocation(
        Invocation, PCHContainerOps,
        CompilerInstance::createDiagnostics(&Invocation->getDiagnosticOpts(),
                                            DiagConsumer,
                                            /*ShouldOwnClient=*/false));
    if (!AST)
      return false;

    ASTs.push_back(std::move(AST));
    return true;
  }
};

}

int ClangTool_::buildASTs(std::vector<std::unique_ptr<ASTUnit>> &ASTs) {
  ASTBuilderAction Action(ASTs);
  return run(&Action);
}

} // end namespace tooling
} // end namespace clang
