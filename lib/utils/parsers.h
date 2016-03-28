#ifndef LIB_UTILS_PARSERS_H_
#define LIB_UTILS_PARSERS_H_

#include <llvm/ADT/StringRef.h>
#include <string>
#include <vector>

#include "../refactor-cpp-export.h"

namespace Refactor {

struct FuncDef {
  std::string name;
};

namespace Parsers {

FuncDef parseFunctionDefinition(const std::string& def);

bool isIdentifier(const std::string& value);

REFACTOR_EXPORT
std::vector<std::string> splitIdentifer(llvm::StringRef id);

}

} // namespace Refactor

#endif /* LIB_UTILS_PARSERS_H_ */
