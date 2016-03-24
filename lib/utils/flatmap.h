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

#ifndef LIB_UTILS_FLATMAP_H_
#define LIB_UTILS_FLATMAP_H_

#include <vector>

namespace Refactor {
namespace Support {

/// \brief key with value
template<typename Key, typename Value>
struct KeyValue
{
  KeyValue() = default;

  explicit KeyValue(const Key& key_, const Value& value_)
  : key(key_), value(value_)
  { }

  Key key;
  Value value;
};

/// \brief sort \c KeyValue after key
template<typename Key, typename Value>
inline bool operator<(const KeyValue<Key,Value>& lhs, const KeyValue<Key,Value>& rhs)
{
  return lhs.key < rhs.key;
}

/// \brief sort \c KeyValue after key
struct LessKey
{
  template<typename Key, typename Value>
  inline bool operator()(const KeyValue<Key,Value>& lhs, const KeyValue<Key,Value>& rhs)
  {
    return lhs.key < rhs.key;
  }
};

struct EqualKey
{
  template<typename Key, typename Value>
  inline bool operator()(const KeyValue<Key,Value>& lhs, const KeyValue<Key,Value>& rhs)
  {
    return lhs.key == rhs.key;
  }
};

struct EqualKeyValue
{
  template<typename Key, typename Value>
  inline bool operator()(const KeyValue<Key,Value>& lhs, const KeyValue<Key,Value>& rhs)
  {
    return lhs.key == rhs.key && lhs.value == rhs.value;
  }
};

struct EqualValue
{
  template<typename Key, typename Value>
  inline bool operator()(const KeyValue<Key,Value>& lhs, const KeyValue<Key,Value>& rhs)
  {
    return lhs.value == rhs.value;
  }
};

template<typename Key, typename Value>
using FlatMultiMap = std::vector<KeyValue<Key, Value>>;

} // namespace Support
} // namespace Refactor

#endif /* LIB_UTILS_FLATMAP_H_ */
