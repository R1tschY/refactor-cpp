
#define BOOST_PP_VARIADICS 1

#include "../utils/parsers.h"

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <regex>

#include "../namestyle.h"

BOOST_FUSION_ADAPT_STRUCT(
  Refactor::FuncDef,
  (std::string, name)
)

//using KeyValueMap = std::map<std::string,std::string>;
//BOOST_FUSION_ADAPT_STRUCT(
//  Refactor::CommandOptions,
//  (KeyValueMap, options)
//  (std::vector<std::string>, arguments)
//)

namespace Refactor {

namespace {

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using x3::int_;
using x3::lit;
using x3::double_;
using x3::lexeme;
using x3::alpha;
using x3::alnum;
using x3::lower;
using x3::upper;
using x3::digit;
using x3::string;
using ascii::char_;

x3::rule<class funcdef, FuncDef> const funcdef = "funcdef";
//x3::rule<class identifier_match, FuncDef> const identifier_match = "identifier";
//x3::rule<class commandoptions, std::vector<std::string>> const commandoptions = "commandoptions";

auto const identifier = lexeme[(alpha | char_('_')) >> *(alnum | char_('_'))];
auto const value = lexeme[*(alnum | "_:%!?&|<>/\\")];
auto const double_colon = lexeme[lit(':') > lit(':')];
auto const funcdef_def = +(lexeme[double_colon > identifier]);

auto const lower_alnum = lexeme[lower | digit];
auto const upper_alnum = lexeme[upper | digit];

auto const lower_case = lexeme[+lower];
auto const upper_case = lexeme[+upper];
auto const camel_case = lexeme[
  upper > ((+lower_alnum) | (+upper_alnum))
];

// general_id_seperator

x3::rule<
  class general_id_seperator,
  std::vector<std::string>
> const general_id_seperator = "general_id_seperator";
/*
auto const general_id_seperator_def =
  // prefix
  (*~alnum)

  // seperated parts (lower case, upper case, camel case)
  > (+lower_alnum | +upper_alnum | (upper >> *lower_alnum))
  % (*~alnum)

  // suffix
  > (*~alnum)
;*/

auto const general_id_seperator_def = lexeme[
  (+lower | (upper >> *lower) | +upper) % *char_('_')
];

//auto const lcc_id_def = lexeme[_a > lower_case > *(camel_case) > _b];
//auto const ucc_id_def = lexeme[_a > camel_case > _b];
//auto const lc_id_def  = lexeme[_a > lower_case % char_(_c) > _b];
//auto const uc_id_def  = lexeme[_a > upper_case % char_(_c) > _b];

//auto const identifier_match_def = x3::matches[identifier];

//auto const command_opt = identifier >> -("=" >> value);

//auto const commandoptions_def =
//  ('[' >> (command_opt % ',') >> ']') >> (value % ',');

// X3 in Boost 1.58 sucks
template <typename Iterator, typename Context, typename Attribute>
inline bool parse_rule(
    decltype(funcdef) rule_
  , Iterator& first, Iterator const& last
  , Context const& context, Attribute& attr)
{
    using boost::spirit::x3::unused;
    auto const& def_ = (funcdef = funcdef_def);
    return def_.parse(first, last, context, unused, attr);
}
template <typename Iterator, typename Context, typename Attribute>
inline bool parse_rule(
    decltype(general_id_seperator) rule_
  , Iterator& first, Iterator const& last
  , Context const& context, Attribute& attr)
{
    using boost::spirit::x3::unused;
    auto const& def_ = (general_id_seperator = general_id_seperator_def);
    return def_.parse(first, last, context, unused, attr);
}

}

FuncDef Parsers::parseFunctionDefinition(const std::string& def)
{
  namespace x3 = boost::spirit::x3;
  namespace ascii = boost::spirit::x3::ascii;

  auto iter = def.cbegin();
  auto end = def.cend();

  FuncDef result;
  bool r = phrase_parse(iter, end, funcdef, ascii::space, result);
  if (!r || iter != end)
  {
    throw std::runtime_error("function definition parse failed");
  }
  return result;
}

std::vector<std::string> Parsers::splitIdentifer(llvm::StringRef id)
{
  namespace x3 = boost::spirit::x3;
  namespace ascii = boost::spirit::x3::ascii;

  const char* iter = id.begin();
  const char* end = id.end();

  std::vector<std::string> result;
  bool r = phrase_parse(iter, end, general_id_seperator, ascii::space, result);
  if (!r || iter != end)
  {
    throw std::runtime_error("function definition parse failed");
  }
  return result;
}

bool Parsers::isIdentifier(const std::string& value)
{
  static std::regex identifier_regex("[a-zA-Z]*[a-zA-Z0-9_]");
  return std::regex_match(value.cbegin(), value.cend(), identifier_regex);
}

} // namespace Refactor
