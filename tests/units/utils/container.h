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

#ifndef TESTS_UNITS_UTILS_CONTAINER_H_
#define TESTS_UNITS_UTILS_CONTAINER_H_

#include <boost/test/tools/assertion_result.hpp>
#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

namespace Refactor {
namespace UnitTests {

template<typename Container>
std::string print_joined(const Container& c, const std::string& sep)
{
  auto first = std::begin(c);
  auto end = std::end(c);

  std::ostringstream s;
  if (first != end)
  {
    s << *first;
  }

  for (++first; first != end; ++first)
  {
    s << sep << *first;
  }

  return s.str();
}

template<typename T1, typename T2>
boost::test_tools::predicate_result
test_container(
  std::initializer_list<T1> exp,
  std::vector<T2> act
)
{
  if (exp.size() != act.size())
  {
    boost::test_tools::predicate_result result(false);
    result.message() << "Different sizes [" << exp.size() << "!=" << act.size() << "]\n";
    result.message() << "expected {" << print_joined(exp, ", ") << "}\n";
    result.message() << "actual {" << print_joined(act, ", ") << "}";
    return result;
  }

  auto iters = std::mismatch(exp.begin(), exp.end(), act.begin());
  if (iters.second != act.end())
  {
    boost::test_tools::predicate_result result(false);
    result.message() << "Different element [" << *iters.first << "!=" << *iters.second << "]\n";
    result.message() << "expected {" << print_joined(exp, ", ") << "}\n";
    result.message() << "actual {" << print_joined(act, ", ") << "}";
    return result;
  }

  return boost::test_tools::predicate_result(true);
}

#define CHECK_CONTAINERS(exp, act) BOOST_CHECK_MESSAGE(test_container((exp), (act)), BOOST_TEST_STRINGIZE(exp == act))

template<typename T>
auto list(std::initializer_list<T> l) -> std::initializer_list<T>
{
  return l;
}

template<typename T>
std::initializer_list<T> empty_list()
{
  return {};
}

} // namespace UnitTests
} // namespace Refactor

#endif /* TESTS_UNITS_UTILS_CONTAINER_H_ */
