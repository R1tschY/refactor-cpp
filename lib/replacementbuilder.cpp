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

#include "replacementbuilder.h"

#include "replacementgroups.h"
#include "utils/algorithm.h"

namespace Refactor {

ReplacementBuilder::ReplacementBuilder(
  Confidence confidence,
  llvm::StringRef description,
  llvm::StringRef source,
  const clang::SourceManager& sources,
  ReplacementGroups& replacements)
:
  sources_(sources),
  replacements_(replacements)
{
  replacement_.setConfidence(confidence);
}

ReplacementBuilder::~ReplacementBuilder()
{
  // TODO: warning or assert if replacement_ is empty
  replacements_.push_back(std::move(replacement_));
}

void ReplacementBuilder::replace(
  const clang::CharSourceRange& Range,
  llvm::StringRef ReplacementText)
{
  replacement_.addReplacement(clang::tooling::Replacement(
    sources_, Range, ReplacementText
  ));
}

void ReplacementBuilder::replace(clang::tooling::Replacement&& replacement)
{
  replacement_.addReplacement(replacement);
}

} // namespace Refactor


