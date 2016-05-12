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

#ifndef LIB_UTILS_STRINGS_H_
#define LIB_UTILS_STRINGS_H_

#include <llvm/ADT/StringRef.h>
#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

#include "algorithm.h"

namespace Refactor {
namespace Support {

template<typename Func>
void split(llvm::StringRef string, char sep, Func&& func)
{
  while (!string.empty())
  {
    auto parts = string.split(sep);

    func(parts.first);
    string = parts.second;
  }
}

template<typename Func>
void split(llvm::StringRef string, llvm::StringRef seps, Func&& func)
{
  while (!string.empty())
  {
    auto parts = string.split(seps);

    func(parts.first);
    string = parts.second;
  }
}

namespace details {

template<typename StringRange>
std::size_t string_length(const StringRange& range)
{ return range.size(); }

inline
std::size_t string_length(const char* range)
{ return strlen(range); }

inline
std::size_t string_length(char c)
{ return 1; }

template<typename StringRange>
void append(std::string& s, const StringRange& range)
{ s.append(std::begin(range), std::end(range)); }

inline
void append(std::string& s, const char* range)
{ s.append(range); }

inline
void append(std::string& s, char c)
{ s.push_back(c); }

}  // namespace details

template<typename...Strings>
std::string concat(Strings&&...strings)
{
  // compute size
//  std::size_t len = 0;
//  foreach_argument(
//    [&len](auto arg){ len += details::string_length(arg); },
//    std::forward<Strings>(strings)...
//  );

  std::size_t len = accumulate_args(
    [](auto arg){ return details::string_length(arg); },
    0,
    std::forward<Strings>(strings)...
  );

  // reserve size
  std::string result;
  result.reserve(len);

  // build string
  foreach_argument(
    [&result](auto arg){ details::append(result, arg); },
    std::forward<Strings>(strings)...
  );

  return result;
}

} // namespace Support
} // namespace Refactor



#endif /* LIB_UTILS_STRINGS_H_ */
