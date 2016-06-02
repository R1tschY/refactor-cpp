#include "replacementgroups.h"

#include "utils/algorithm.h"

namespace Refactor {

void ReplacementGroup::addReplacement(const Replacement& replacement)
{
  replacements_.push_back(replacement);
}

void ReplacementGroup::addReplacement(Replacement&& replacement)
{
  replacements_.push_back(std::move(replacement));
}

std::vector<Replacement> getReplacements(
  const ReplacementGroups& groups)
{
  std::vector<Replacement> result;

  for (auto& group : groups)
  {
    auto& replacements = group.getReplacements();
    Support::append(result, replacements);
  }

  return result;
}

} // namespace Refactor
