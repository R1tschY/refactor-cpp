#ifndef LIB_UTILS_PARSERS_H_
#define LIB_UTILS_PARSERS_H_

#include <map>
#include <string>
#include <vector>

namespace Refactor {

struct FuncDef {
  std::string name;
};

namespace Parsers {

FuncDef parseFunctionDefinition(const std::string& def);

bool isIdentifier(const std::string& value);

}

} // namespace Refactor

#endif /* LIB_UTILS_PARSERS_H_ */
