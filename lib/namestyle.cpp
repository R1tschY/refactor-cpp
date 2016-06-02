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

#define BOOST_PP_VARIADICS 1

#include "namestyle.h"

#include <iostream>
#include <boost/spirit/home/x3.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptors.hpp>
#include <llvm/ADT/StringMap.h>

#include "utils/algorithm.h"

namespace Refactor {

namespace {

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using namespace boost::spirit::x3;
using namespace boost::spirit::x3::ascii;

using x3::int_;
using x3::lit;
using x3::double_;
using x3::lexeme;
using x3::no_skip;
using x3::alpha;
using x3::alnum;
using x3::digit;
using x3::lower;
using x3::upper;
using x3::digit;
using x3::string;
using ascii::char_;

// identifier case

auto const lower_alnum = lower | digit;
auto const upper_alnum = upper | digit;

auto const id_sep = x3::rule<class s2_, x3::unused_type>("id_sep")
  = omit[~alnum];

auto const lower_case = x3::rule<class lower_case_, std::string>("lower_case")
  = lexeme[+lower_alnum];
auto const upper_case = x3::rule<class upper_case_, std::string>("upper_case")
  = lexeme[+upper_alnum];
auto const camel_case = x3::rule<class camel_case_, std::string>("camel_case")
  = lexeme[upper >> (+lower_alnum)];


x3::rule<
  class lower_case_id, std::vector<std::string>
> const lower_case_id = "lower_case_id";
auto const lower_case_id_def = no_skip[ lower_case % id_sep ];
BOOST_SPIRIT_DEFINE(lower_case_id);

x3::rule<
  class upper_case_id, std::vector<std::string>
> const upper_case_id = "upper_case_id";
auto const upper_case_id_def = no_skip[ upper_case % id_sep ];
BOOST_SPIRIT_DEFINE(upper_case_id);

x3::rule<
  class lower_camel_case_id, std::vector<std::string>
> const lower_camel_case_id = "lower_camel_case_id";
auto const lower_camel_case_id_def = no_skip[ lower_case > *camel_case ];
BOOST_SPIRIT_DEFINE(lower_camel_case_id);

x3::rule<
  class upper_camel_case_id, std::vector<std::string>
> const upper_camel_case_id = "upper_camel_case_id";
auto const upper_camel_case_id_def = no_skip[ +camel_case ];
BOOST_SPIRIT_DEFINE(upper_camel_case_id);

// general_id_seperator

x3::rule<
  class general_id_seperator,
  std::vector<std::string>
> const general_id_seperator = "general_id_seperator";

auto const gid_upper_case = x3::rule<class upper_case_, std::string>("gid_upper_case")
  = lexeme[+((upper >> !lower | digit))];

auto const gid = x3::rule<class s1_, std::string>("gid")
  = lower_case | camel_case | gid_upper_case;

auto const first_id = x3::rule<class s2_, std::string>("first_id")
  = (+digit >> -gid) | gid;

auto const nth_id = x3::rule<class s3_, std::string>("nth_id")
  = gid | (+id_sep >> first_id);

auto const general_id_seperator_def =
  no_skip[
    *id_sep
    > first_id
    > *nth_id
    > *id_sep
  ];
BOOST_SPIRIT_DEFINE(general_id_seperator);

//

template <typename Parser, typename Skipper, typename Attribute>
bool try_parse(llvm::StringRef input, const Parser& p, const Skipper& skipper, Attribute& attr)
{
  const char* iter = input.begin();
  const char* end = input.end();

  try
  {
    bool r = x3::phrase_parse(iter, end, p, skipper, attr);
    return (r && iter == end);
  }
  catch (...)
  {
    return false;
  }
}

template <typename Parser, typename Skipper>
bool try_parse(llvm::StringRef input, const Parser& p, const Skipper& skipper)
{
  return try_parse(input, p, skipper, x3::unused);
}

}

std::vector<std::string> splitIdentifer(llvm::StringRef id, const TypeNameStyle& guessed_style, bool* ok)
{
  namespace x3 = boost::spirit::x3;

  if (ok)
    *ok = true;

  std::vector<std::string> result;

  // remove existing prefix
  if (id.startswith(guessed_style.prefix))
    id.slice(guessed_style.prefix.size(), llvm::StringRef::npos);

  // remove existing suffix
  if (id.endswith(guessed_style.suffix))
    id.slice(0, id.size() - guessed_style.prefix.size());

  // TODO: use seperator and case information

  // last chance
  return splitIdentifer(id, ok);
}

std::vector<std::string> splitIdentifer(llvm::StringRef id, bool* ok)
{
  namespace x3 = boost::spirit::x3;

  if (ok)
    *ok = true;

  std::vector<std::string> result;

  // special cases
  if (id.size() == 0)
    return result;
  if (id.size() == 1)
    return result = { std::string(1, escapeIdentifierChar(id[0])) };

  bool r = try_parse(id, general_id_seperator, x3::ascii::space, result);
  if (!r)
  {
    if (ok)
      *ok = false;
    return result = { std::string(id.begin(), id.end()) };
  }
  return result;
}

static std::string toCamelCase(llvm::StringRef id)
{
  std::string result = id.lower();
  if (result.size() > 0)
    result[0] = static_cast<char>(std::toupper(result[0]));
  return result;
}

static std::string convertIdentifierPart(llvm::StringRef id, std::size_t n, Capitalization cap)
{
  switch (cap)
  {
  case Capitalization::LowerCase:
    return id.lower();

  case Capitalization::UpperCase:
    return id.upper();

  case Capitalization::UpperCamelCase:
    return toCamelCase(id);

  case Capitalization::LowerCamelCase:
    return n == 0 ? id.lower() : toCamelCase(id);

  default:
  case Capitalization::Original:
    return std::string(id.begin(), id.end());
  }
}

std::string joinIdentifier(const std::vector<std::string>& parts, const TypeNameStyle& style)
{
  std::string result;

  // prefix
  result += style.prefix;

  // parts
  // TODO: build own faster/better version
  // TODO: use llvm::join in StringExtras.h?
  std::size_t n = 0;
  result += boost::algorithm::join(
    boost::adaptors::transform(parts, [&](const std::string& a){
      return convertIdentifierPart(a, n++, style.capitalization);
    }),
    style.word_delimiter);

  // suffix
  result += style.suffix;

  return result;
}

namespace {

struct NameStyleRegistry
{
  NameStyleRegistry();

  NameStyleRegistry(const NameStyleRegistry&) = delete;
  NameStyleRegistry& operator=(const NameStyleRegistry&) = delete;

  static NameStyleRegistry& get();

  void registerNameStyle(llvm::StringRef id, NameStyleFactory factory)
  {
    name_styles_[id] = factory;
  }

  NameStyle create(llvm::StringRef id) const;
  std::vector<std::string> getRegisteredNameStyles() const;

private:
  llvm::StringMap<NameStyleFactory> name_styles_;
};

NameStyleRegistry::NameStyleRegistry()
{
  // register default styles
  registerNameStyle(
    "boost",
    [](llvm::StringRef id)
    {
      NameStyle result;

      TypeNameStyle common_style;
      common_style.capitalization = Capitalization::LowerCase;
      common_style.word_delimiter = "_";

      // RECORDS

      result.records = common_style;

      // FUNCTIONS

      result.methods = common_style;
      result.static_method = common_style;
      result.function = common_style;

      result.getter = common_style;
      result.bool_getter = common_style;
      result.setter = common_style;

      // NAMESPACE

      result.namespace_ = common_style;

      // VARIABLES

      result.variable = common_style;
      result.field = common_style;
      result.constant = common_style;
      result.parameter = common_style;
      result.template_parameter = common_style;

      // MACROS

      result.macro = common_style;
      result.macro.capitalization = Capitalization::UpperCase;

      // FILES

      // source file
      result.source_file = common_style;
      result.source_file.suffix = ".cpp";

      // header file
      result.header_file = common_style;
      result.header_file.suffix = ".hpp";

      // test file
      result.test_file = common_style;
      result.test_file.suffix = "_test.cpp";

      // include guard

      result.include_guard.name_style.capitalization = Capitalization::UpperCase;
      result.include_guard.type = IncludeGuardStyleType::Path;

      return result;
    }
  );

  // register default styles
  registerNameStyle(
    "llvm",
    [](llvm::StringRef id)
    {
      NameStyle result;

      TypeNameStyle common_style;
      common_style.capitalization = Capitalization::UpperCamelCase;

      // RECORDS

      result.records = common_style;

      // FUNCTIONS

      TypeNameStyle func_style;
      func_style.capitalization = Capitalization::LowerCamelCase;

      result.methods = func_style;
      result.static_method = func_style;
      result.function = func_style;

      result.getter = func_style;
      result.getter.prefix = "get";

      result.bool_getter = func_style;
      result.bool_getter.prefix = "is";

      result.setter = func_style;
      result.setter.prefix = "set";

      // NAMESPACE

      result.namespace_.capitalization = Capitalization::LowerCase;
      result.namespace_.word_delimiter = "_";

      // VARIABLES

      result.variable = common_style;
      result.field = common_style;
      result.constant = common_style;
      result.parameter = common_style;
      result.template_parameter = common_style;

      // MACROS

      result.macro = common_style;
      result.macro.capitalization = Capitalization::UpperCase;

      // FILES

      // source file
      result.source_file = common_style;
      result.source_file.suffix = ".cpp";

      // header file
      result.header_file = common_style;
      result.header_file.suffix = ".h";

      // test file
      result.test_file = common_style;
      result.test_file.suffix = "_test.cpp";

      // include guard

      result.include_guard.name_style.capitalization = Capitalization::UpperCase;
      result.include_guard.type = IncludeGuardStyleType::Path;

      return result;
    }
  );
}

NameStyleRegistry& NameStyleRegistry::get()
{
  static NameStyleRegistry instance;
  return instance;
}

NameStyle NameStyleRegistry::create(llvm::StringRef id) const
{
  // search
  auto iter = name_styles_.find(id);

  // existing
  if (iter == name_styles_.end()) {
    std::ostringstream s;
    s << "name style with id `" << id << "` is not existing";
    throw std::invalid_argument(s.str());
  }

  // create
  return iter->second(id);
}

std::vector<std::string> NameStyleRegistry::getRegisteredNameStyles() const
{
  std::vector<std::string> result;
  for (const auto& e : name_styles_)
  {
    result.push_back(e.first());
  }
  return result;
}

}

void registerNameStyle(llvm::StringRef id, NameStyleFactory factory)
{
  assert(id);
  assert(factory);
  NameStyleRegistry::get().registerNameStyle(id, std::move(factory));
}

NameStyle getNameStyle(llvm::StringRef id)
{
  return NameStyleRegistry::get().create(id);
}

std::vector<std::string> getRegisteredNameStyles()
{
  return NameStyleRegistry::get().getRegisteredNameStyles();
}

} // namespace Refactor


