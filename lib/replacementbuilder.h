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

#ifndef LIB_REPLACEMENTBUILDER_H_
#define LIB_REPLACEMENTBUILDER_H_

#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Tooling/Core/Replacement.h>
#include <llvm/ADT/StringRef.h>

#include "replacementgroups.h"

namespace Refactor {

/// \brief
class ReplacementBuilder
{
public:
  ReplacementBuilder(
    Confidence confidence,
    llvm::StringRef description,
    llvm::StringRef source,
    const clang::SourceManager& sources,
    ReplacementGroups& replacements);

  ~ReplacementBuilder();

  void replace(const clang::CharSourceRange& Range, llvm::StringRef ReplacementText);
  void replace(clang::tooling::Replacement&& replacement);

private:
  ReplacementGroup replacement_;
  const clang::SourceManager& sources_;
  ReplacementGroups& replacements_;
};

} // namespace Refactor

#endif /* LIB_REPLACEMENTBUILDER_H_ */
