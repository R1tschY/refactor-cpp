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

#ifndef LIB_INTERNALS_VERSION_H_
#define LIB_INTERNALS_VERSION_H_

#include "../utils/shortcuts.h"

namespace Refactor {

class Version
{
public:
  constexpr Version() { }
  constexpr Version(unsigned major) : major_(major) { }
  constexpr Version(unsigned major, unsigned minor)
    : major_(major), minor_(minor)
  { }
  constexpr Version(unsigned major, unsigned minor, unsigned patch)
    : major_(major), minor_(minor), patch_(patch)
  { }

  constexpr unsigned getMajor() const { return major_; }
  constexpr unsigned getMinor() const { return minor_; }
  constexpr unsigned getPatch() const { return patch_; }

  constexpr void setMajor(unsigned major) { major_ = major; }
  constexpr void setMinor(unsigned minor) { minor_ = minor; }
  constexpr void setPatch(unsigned patch) { patch_ = patch; }

private:
  unsigned major_ = 0;
  unsigned minor_ = 0;
  unsigned patch_ = 0;
};

class ExtendedVersion : public Version
{
public:
  using Version::Version;

  ExtendedVersion(unsigned major, unsigned minor, unsigned patch, StringRef string)
   : Version(major, minor, patch), string_version_(string)
  { }

  /// \brief set string version
  void setStringVersion(StringRef value) { string_version_ = value; }

  /// \brief get string version
  StringRef getStringVersion() const { return string_version_; }

private:
  std::string string_version_ = { };
};

} // namespace Refactor

#endif /* LIB_INTERNALS_VERSION_H_ */
