/// \file namestyle.h

#ifndef LIB_NAMESTYLE_H_
#define LIB_NAMESTYLE_H_

namespace Refactor {

enum class TypeNameClass
{
  Record,
  Method,
  StaticMethod,
  Function,
  Namespace,
  Member,
  Constant,
  Macro,
  Parameter,
  TemplateParameter,
  SourceFile,
  HeaderFile,
  TestFile
};

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
  std::map<TypeNameClass, TypeNameStyle> variant1;
  // OR:

};

}  // namespace Refactor

#endif /* LIB_NAMESTYLE_H_ */
