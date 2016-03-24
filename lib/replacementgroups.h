#ifndef LIB_REPLACEMENTGROUPS_H_
#define LIB_REPLACEMENTGROUPS_H_

#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Tooling/Core/Replacement.h>
#include <llvm/ADT/StringRef.h>
#include <vector>

#include "utils/shortcuts.h"

namespace Refactor {

enum class Confidence
{
  Safe,
  Unsafe
};

class ReplacementGroup
{
public:
  typedef llvm::SmallVector<Replacement, 1> Container;
  typedef llvm::SmallVectorImpl<Replacement> ContainerInterface;

  void setConfidence(Confidence certainty) { confidence_ = certainty; }
  Confidence getConfidence() const { return confidence_; }

  void addReplacement(const Replacement& replacement);
  void addReplacement(Replacement&& replacement);
  const ContainerInterface& getReplacements() const { return replacements_; }

private:
  Container replacements_;
  Confidence confidence_ = Confidence::Unsafe;
};

typedef std::vector<ReplacementGroup> ReplacementGroups;

std::vector<Replacement> getReplacements(const ReplacementGroups& groups);

} // namespace Refactor

#endif /* LIB_REPLACEMENTGROUPS_H_ */
