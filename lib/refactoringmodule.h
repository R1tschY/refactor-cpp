///
/// Copyright (c) 2016 R1tschY
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to 
/// deal in the Software without restriction, including without limitation the 
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#ifndef LIB_REFACTORINGMODULE_H_
#define LIB_REFACTORINGMODULE_H_

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <functional>
#include <memory>

#include "hook.h"
#include "refactoring.h"
#include "refactoringargs.h"


namespace Refactor {


/// \brief factory method to create a refactoring
typedef std::function<
          std::unique_ptr<Refactoring>(
            llvm::StringRef name,
            const RefactoringArgs& args
          )
        > RefactoringFactory;

/// \brief factory for refactoring features
class RefactoringFactories
{
public:
  typedef llvm::StringMap<RefactoringFactory> FactoryMap;

  /// \brief register a refactoring feature from a factory method
  void registerRefactoring(llvm::StringRef name, RefactoringFactory factory);

  /// \brief register a refactoring feature from a class
  template<typename T>
  void registerRefactoring(llvm::StringRef module_name);

  /// \brief create a reafctoring from a \p name and \p args
  std::unique_ptr<Refactoring> create(
    llvm::StringRef name, const RefactoringArgs& args);

  // read-only container support

  FactoryMap::const_iterator begin() const { return factories_.begin(); }
  FactoryMap::const_iterator end() const { return factories_.end(); }
  bool empty() const { return factories_.empty(); }
  bool size() const { return factories_.size(); }

private:
  FactoryMap factories_;
};

/// \brief interface for modules to intergrate to system
class RefactoringModule
{
public:
  virtual ~RefactoringModule() = default;

  virtual void addFactories(RefactoringFactories& factories) = 0;
  virtual void addHooks(HookRegistry& hooks) { };
};

// implementation

template<typename T>
void RefactoringFactories::registerRefactoring(llvm::StringRef module_name)
{
  registerRefactoring(
    module_name,
    [](llvm::StringRef name, const RefactoringArgs& args)
    {
      return std::make_unique<T>(name, args);
    }
  );
}

} // namespace Refactor

#endif /* LIB_REFACTORINGMODULE_H_ */
