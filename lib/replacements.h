#ifndef LIB_REPLACEMENTS_H_
#define LIB_REPLACEMENTS_H_

#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Tooling/Core/Replacement.h>
#include <llvm/ADT/StringRef.h>
#include <vector>

namespace Refactor {

enum class Certainty
{
  Safe,
  Unsafe
};

class Replacement
{
public:
  template<typename...Args>
  Replacement(Args&&...args)
  : replacement_(std::forward<Args>(args)...)
  { }

  template<typename...Args>
  Replacement(Certainty certainty, Args&&...args)
  : replacement_(std::forward<Args>(args)...),
    certainty_(certainty)
  { }

  void setCertainty(Certainty certainty) { certainty_ = certainty; }
  Certainty getCertainty() const { return certainty_; }

  const clang::tooling::Replacement& getReplacement() const { return replacement_; }

private:
  clang::tooling::Replacement replacement_;
  Certainty certainty_ = Certainty::Unsafe;
};

inline
bool operator<(const Replacement& lhs, const Replacement& rhs)
{
  return lhs.getReplacement() < rhs.getReplacement();
}

inline
bool operator==(const Replacement& lhs, const Replacement& rhs)
{
  return lhs.getReplacement() == rhs.getReplacement();
}

/// \brief
class Replacements
{
public:
  Replacements();

  void addReplacement(
    Certainty certainty,
    const clang::SourceManager& sources,
    const clang::CharSourceRange& range,
    llvm::StringRef replacement_text);

  void deduplicate();

  std::vector<clang::tooling::Replacement> getRawReplacements() const;
  const std::vector<Replacement>& get() const { return replacements_; }

private:
  std::vector<Replacement> replacements_;
};

} // namespace Refactor

#endif /* LIB_REPLACEMENTS_H_ */
