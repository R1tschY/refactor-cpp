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

#ifndef LIB_REFACTORINGARGS_H_
#define LIB_REFACTORINGARGS_H_

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <algorithm>
#include <string>
#include <vector>

namespace Refactor {


class RefactoringArgs
{
public:
  typedef std::vector<std::string> ArgsContainer;
  typedef llvm::StringMap<std::string> OptionsContainer;

  RefactoringArgs() = default;
  RefactoringArgs(ArgsContainer&& args, OptionsContainer&& options)
  : args_(std::move(args)), options_(std::move(options))
  { }

  static RefactoringArgs fromString(llvm::StringRef args_str);

  const ArgsContainer& getArgs() const { return args_; }
  const OptionsContainer& getOptions() const { return options_; }

private:
  ArgsContainer args_;
  OptionsContainer options_;
};

} // namespace Refactor

#endif /* LIB_REFACTORINGARGS_H_ */
