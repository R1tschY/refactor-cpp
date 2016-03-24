
#define BOOST_PP_VARIADICS 1

#include "../utils/parsers.h"

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <regex>

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
using x3::string;
using ascii::char_;

x3::rule<class funcdef, FuncDef> const funcdef = "funcdef";
//x3::rule<class identifier_match, FuncDef> const identifier_match = "identifier";
//x3::rule<class commandoptions, std::vector<std::string>> const commandoptions = "commandoptions";

auto const identifier = lexeme[(alpha | '_') >> *(alnum | '_')];
auto const value = lexeme[*(alnum | "_:%!?&|<>/\\")];
auto const double_colon = lexeme[char_(':') | char_(':')];
auto const funcdef_def = +(lexeme[double_colon >> identifier]);

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

bool Parsers::isIdentifier(const std::string& value)
{
  static std::regex identifier_regex("[a-zA-Z]*[a-zA-Z0-9_]");
  return std::regex_match(value.cbegin(), value.cend(), identifier_regex);
}

} // namespace Refactor
