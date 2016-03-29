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

#ifndef TESTS_UNITS_UTILS_STRINGGENERATOR_H_
#define TESTS_UNITS_UTILS_STRINGGENERATOR_H_

#include <llvm/ADT/StringRef.h>
#include <cstddef>
#include <random>
#include <string>

namespace Refactor {
namespace UnitTests {

template<typename T>
struct Range
{
  Range(const T& start_, const T& end_)
  : start(start_), end(end_)
  { }

  T start;
  T end;
};

/// \brief
class StringGenerator
{
public:
  StringGenerator(llvm::StringRef chars, Range<std::size_t> size_range);

  std::string operator()(std::mt19937& rg);

private:
  std::string chars_;
  std::uniform_int_distribution<std::size_t> char_dist_;
  std::uniform_int_distribution<std::size_t> n_dist_;
};

} // namespace UnitTests
} // namespace Refactor

#endif /* TESTS_UNITS_UTILS_STRINGGENERATOR_H_ */
