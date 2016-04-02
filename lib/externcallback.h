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

#ifndef LIB_EXTERNCALLBACK_H_
#define LIB_EXTERNCALLBACK_H_

#include <llvm/ADT/StringRef.h>
#include <cstddef>
#include <memory>
#include <vector>

namespace Refactor {

class ExternCallback
{
public:
  virtual ~ExternCallback() = default;

  virtual void onFileMoved(llvm::StringRef old_name, llvm::StringRef new_name) { }
};

class ExternCallbacks
{
public:
  using Collection = std::vector<std::unique_ptr<ExternCallback>>;

  void registerCallback(std::unique_ptr<ExternCallback> callback);

  void onFileMoved(llvm::StringRef old_name, llvm::StringRef new_name);

  Collection::const_iterator begin() const { return callbacks_.begin(); }
  Collection::const_iterator end() const { return callbacks_.end(); }
  bool empty() const { return callbacks_.empty(); }
  std::size_t size() const { return callbacks_.size(); }

private:
  std::vector<std::unique_ptr<ExternCallback>> callbacks_;
};

} // namespace Refactor

#endif /* LIB_EXTERNCALLBACK_H_ */
