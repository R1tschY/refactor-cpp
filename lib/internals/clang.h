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

#ifndef LIB_INTERNALS_CLANG_H_
#define LIB_INTERNALS_CLANG_H_

#include <string>
#include <llvm/ADT/StringRef.h>

#include "../utils/shortcuts.h"
#include "version.h"

#define REFACTOR_VERSION(major,minor,patch) \
  (major * 10000 + minor * 100 + patch)
#define REFACTOR_LLVM_VERSION \
  REFACTOR_VERSION(LLVM_VERSION_MAJOR, LLVM_VERSION_MINOR, LLVM_VERSION_PATCH)
#define REFACTOR_LLVM_PREQE(major,minor,patch) \
  (REFACTOR_LLVM_VERSION >= REFACTOR_VERSION(major,minor,patch))


namespace Refactor {

/// \brief information about clang installation
class Clang
{
public:
  /// \brief chooses the clang installation which is compiled to
  Clang();

  // llvm

  /// \brief set the prefix of the llvm installation with the clang installation
  void setLLVMPrefix(StringRef prefix);

  /// \brief get the used llvm prefix of the clang installation
  StringRef getLLVMPrefix() const { return llvm_prefix_; }

  /// \brief get the version of LLVM which was linked against
  static ExtendedVersion getLLVMVersion();

  // clang

  /// \brief get resource dir for used clang
  ///
  /// \returns \code getLLVMPrefix()/lib/clang/LLVM_VERSION_STRING \endcode
  std::string getResourceDir() const;

private:
  /// \brief llvm prefix of the clang installation
  std::string llvm_prefix_;
};

} // namespace Refactor

#endif /* LIB_INTERNALS_CLANG_H_ */
