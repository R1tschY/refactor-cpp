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

#include "stringgenerator.h"

namespace Refactor {
namespace UnitTests {

StringGenerator::StringGenerator(llvm::StringRef chars, Range<std::size_t> size_range)
: chars_(chars), char_dist_(0, chars.size()-1), n_dist_(size_range.start, size_range.end)
{ }

std::string StringGenerator::operator ()(std::mt19937& rg)
{
  std::size_t n = n_dist_(rg);

  std::string result;
  result.reserve(n);
  for (std::size_t i = 0; i < n; i++)
  {
    result.push_back(chars_[char_dist_(rg)]);
  }

  return result;
}

} // namespace UnitTests
} // namespace Refactor
