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

#include "refactoring.h"
#include "refactoringargs.h"
#include "externcallback.h"


namespace Refactor {


typedef std::function<
          std::unique_ptr<Refactoring>(
            llvm::StringRef name,
            const RefactoringArgs& args
          )
        > RefactoringFactory;

class RefactoringFactories
{
public:
  typedef llvm::StringMap<RefactoringFactory> FactoryMap;

  void registerRefactoring(llvm::StringRef name, RefactoringFactory factory);

  template<typename T>
  void registerRefactoring(llvm::StringRef name)
  {
    registerRefactoring(
      name,
      [](llvm::StringRef name, const RefactoringArgs& args)
      {
        return std::make_unique<T>(name, args);
      }
    );
  }

  std::unique_ptr<Refactoring> create(
    llvm::StringRef name, const RefactoringArgs& args);

  FactoryMap::const_iterator begin() const { return factories_.begin(); }
  FactoryMap::const_iterator end() const { return factories_.end(); }
  bool empty() const { return factories_.empty(); }
  bool size() const { return factories_.size(); }

private:
  FactoryMap factories_;
};

/// \brief
class RefactoringModule
{
public:
  virtual ~RefactoringModule() = default;

  virtual void addFactories(RefactoringFactories& factories) = 0;
  virtual void addCallback(ExternCallbacks& callbacks) { };
};

} // namespace Refactor

#endif /* LIB_REFACTORINGMODULE_H_ */
