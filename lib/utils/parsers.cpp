#define BOOST_PP_VARIADICS 1

#include "parsers.h"

#include <iostream>
#include <string>
#include <complex>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "algorithm.h"

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
namespace Parsers {

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

// identifier

x3::rule<class identifier, std::string> const identifier = "identifier";
auto const identifier_def = lexeme[(alpha | char_('_')) >> *(alnum | char_('_'))];
BOOST_SPIRIT_DEFINE(identifier);

// Function definition

x3::rule<class funcdef, FuncDef> const funcdef = "funcdef";
auto const funcdef_def = +skip[lit("::") >> identifier];
BOOST_SPIRIT_DEFINE(funcdef);

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

FuncDef parseFunctionDefinition(llvm::StringRef def)
{
  namespace x3 = boost::spirit::x3;
  namespace ascii = boost::spirit::x3::ascii;

  auto iter = def.begin();
  auto end = def.end();

  FuncDef result;
  bool r = phrase_parse(iter, end, funcdef, ascii::space, result);
  if (!r || iter != end)
  {
    throw std::runtime_error("function definition parse failed");
  }
  return result;
}

bool isIdentifier(llvm::StringRef value)
{
  namespace x3 = boost::spirit::x3;

  return try_parse(value, x3::no_skip[x3::omit[identifier]], x3::ascii::space);
}

} // namespace Parsers
} // namespace Refactor
