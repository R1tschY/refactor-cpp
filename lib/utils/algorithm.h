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

#ifndef LIB_UTILS_ALGORITHM_H_
#define LIB_UTILS_ALGORITHM_H_

#include <algorithm>

namespace Refactor {
inline namespace Support {

// meta programming support

template<typename... Ts> struct makeVoid { typedef void type;};
template<typename... Ts> using Void = typename makeVoid<Ts...>::type;

#define REQUIRES(...) typename std::enable_if<(__VA_ARGS__), int>::type = 0

// hasReserve

template<typename, typename = void>
struct hasReserve : std::false_type { };

template<typename T>
struct hasReserve<
  T,
  Void<
    decltype(std::declval<T>().reserve())
  >
> : std::true_type { };

// reserve

template<typename Container, REQUIRES(!hasReserve<Container>())>
void reserve(Container& c, std::size_t size) { }

template<typename Container, REQUIRES(hasReserve<Container>())>
void reserve(Container& c, std::size_t size)
{
  c.reserve(size);
}

/// \brief remove duplicates in container
///
/// container gets sorted before duplicates are removed.
template<typename Container>
void deduplicate(Container& container)
{
  std::sort(container.begin(), container.end());
  container.erase(
    std::unique(container.begin(), container.end()),
    container.end()
  );
}

template<typename Generator>
void runNTimes(std::size_t n, Generator&& g)
{
  for (std::size_t i = n; i != 0; --i)
  {
    g();
  }
}

template<typename In, typename Out, typename Func>
void adjacentDifference(const In& lhs, Out& rhs, Func&& func)
{
  reserve(rhs, lhs.size());
  std::adjacent_difference(lhs.begin(), lhs.end(), std::back_inserter(rhs), std::move(func));
}

/// \brief add copy of elements of \p rhs to \p lhs
/// \details append all elements of rhs to lhs by copying
template<typename ContainerLhs, typename ContainerRhs>
void append(ContainerLhs& lhs, const ContainerRhs& rhs)
{
  lhs.insert(lhs.end(), rhs.begin(), rhs.end());
}

/// \brief add elements of \p rhs to \p lhs
/// \details append all elements of rhs to lhs by moving and clear rhs
template<typename ContainerLhs, typename ContainerRhs>
void merge(ContainerLhs& lhs, ContainerRhs& rhs)
{
  reserve(lhs, lhs.size() + rhs.size());
  std::move(rhs.begin(), rhs.end(), std::back_inserter(lhs));
  rhs.clear();
}

/// \brief copy elements of \p rhs to \p lhs
template<typename ContainerLhs, typename ContainerRhs>
void assign(const ContainerLhs& lhs, const ContainerRhs& rhs)
{
  lhs.assign(rhs.begin(), rhs.end());
}

/// from https://twitter.com/SeanParent/status/558765089294020609
template<typename Fun, typename...Args>
void foreach_argument(Fun&& f, Args&&... args) {
  using t = int[sizeof...(Args)];
  (void)t{(f(std::forward<Args>(args)), 0)...};
}

template<typename Fun, typename...Args>
std::size_t accumulate_args(Fun&& fmap, std::size_t init, Args&&... args) {
  using t = int[sizeof...(Args)];
  (void)t{(init += fmap(std::forward<Args>(args)), 0)...};
  return init;
}

} // namespace Support
} // namespace Refactor

#endif /* LIB_UTILS_ALGORITHM_H_ */
