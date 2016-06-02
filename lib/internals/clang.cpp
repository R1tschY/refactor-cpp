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

#include "clang.h"

#include <llvm/Config/llvm-config.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/Path.h>

namespace Refactor {

Clang::Clang()
: llvm_prefix_(LLVM_PREFIX)
{ }

void Clang::setLLVMPrefix(StringRef prefix)
{
  prefix = llvm_prefix_;
}

ExtendedVersion Clang::getLLVMVersion()
{
  return ExtendedVersion(
    LLVM_VERSION_MAJOR, LLVM_VERSION_MINOR, LLVM_VERSION_PATCH,
    LLVM_VERSION_STRING);
}

std::string Clang::getResourceDir() const
{
  SmallString<128> result;
  sys::path::append(result, llvm_prefix_, "lib", "clang", LLVM_VERSION_STRING);
  return result.str();
}

} // namespace Refactor
