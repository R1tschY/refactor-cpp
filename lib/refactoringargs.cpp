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

#include "refactoringargs.h"

#include <stdexcept>

#include "utils/strings.h"

namespace Refactor {

RefactoringArgs RefactoringArgs::fromString(llvm::StringRef args_str)
{
  // parse args
  ArgsContainer args;
  Support::split(args_str, ',', [&](llvm::StringRef arg)
  {
    arg = arg.trim(" \t\n\r");
    if (arg.empty()) throw std::invalid_argument("empty argument");
    args.emplace_back(arg.begin(), arg.end());
  });

  // todo: parse options
  OptionsContainer options;

  return RefactoringArgs(std::move(args), std::move(options));
}

} // namespace Refactor
