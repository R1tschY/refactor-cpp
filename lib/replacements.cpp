#include "replacements.h"

#include <algorithm>

namespace Refactor {

Replacements::Replacements()
: replacements_()
{
  replacements_.reserve(10);
}

void Replacements::addReplacement(
  Certainty certainty,
  const clang::SourceManager& sources,
  const clang::CharSourceRange& range,
  llvm::StringRef replacement_text)
{
  replacements_.emplace_back(certainty, sources, range, replacement_text);
}

void Replacements::deduplicate()
{
  std::sort(replacements_.begin(), replacements_.end());
  replacements_.erase(
    std::unique(replacements_.begin(), replacements_.end()),
    replacements_.end());
}

std::vector<clang::tooling::Replacement> Replacements::getRawReplacements() const
{
  std::vector<clang::tooling::Replacement> result;
  result.reserve(replacements_.size());
  for (const auto& replacement : replacements_)
  {
    result.push_back(replacement.getReplacement());
  }
  return result;
}

} // namespace Refactor
