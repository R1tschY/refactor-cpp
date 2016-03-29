/// \file namestyle.h

#ifndef LIB_NAMESTYLE_H_
#define LIB_NAMESTYLE_H_

#include <string>
#include <cctype>
#include <cstddef>
#include <vector>
#include <functional>
#include <llvm/ADT/StringRef.h>

#include "refactor-cpp-export.h"

namespace Refactor {

/// \brief Capitalization of name
///
///
enum class Capitalization
{
  /// \brief do not change case of input
  ///
  /// \note cannot be applied everywhere
  Original,

  /// \brief first letter of every word up
  ///
  /// examples: \c MyClass, \c MyAPI, \c Base64
  UpperCamelCase,

  /// \brief first letter of every word up but not the first letter
  ///
  /// examples: \c myClass, \c myAPI, \c base64
  LowerCamelCase,

  /// \brief only lower case letters
  LowerCase,

  /// \brief only upper case letters
  UpperCase
};

struct TypeNameStyle
{
  Capitalization capitalization;
  std::string word_delimiter;
  std::string suffix;
  std::string prefix;
};

class NameStyle
{
public:
  TypeNameStyle namespace_;

  TypeNameStyle records;

  TypeNameStyle methods;
  TypeNameStyle static_method;
  TypeNameStyle function;
  TypeNameStyle getter;
  TypeNameStyle setter;
  TypeNameStyle bool_getter;

  TypeNameStyle variable;
  TypeNameStyle field;
  TypeNameStyle constant;
  TypeNameStyle parameter;
  TypeNameStyle template_parameter;

  TypeNameStyle macro;

  TypeNameStyle source_file;
  TypeNameStyle header_file;
  TypeNameStyle test_file;
};

inline
char escapeIdentifierChar(char id)
{
  return std::isalnum(id) ? id : '_';
}

REFACTOR_EXPORT
std::string joinIdentifier(const std::vector<std::string>& parts, const TypeNameStyle& style);

REFACTOR_EXPORT
std::vector<std::string> splitIdentifer(llvm::StringRef id, bool* ok = nullptr);

using NameStyleFactory = std::function<NameStyle(llvm::StringRef)>;

void registerNameStyle(llvm::StringRef id, NameStyleFactory factory);

NameStyle getNameStyle(llvm::StringRef id);
std::vector<std::string> getRegisteredNameStyles();

}  // namespace Refactor

#endif /* LIB_NAMESTYLE_H_ */
