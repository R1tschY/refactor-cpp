#ifndef LIB_UTILS_PARSERS_H_
#define LIB_UTILS_PARSERS_H_

#include <llvm/ADT/StringRef.h>

#include "../refactor-cpp-export.h"

namespace Refactor {

struct FuncDef {
  std::string name;
};

namespace Parsers {

REFACTOR_EXPORT
FuncDef parseFunctionDefinition(llvm::StringRef def);

REFACTOR_EXPORT
bool isIdentifier(llvm::StringRef value);

}

} // namespace Refactor

#endif /* LIB_UTILS_PARSERS_H_ */
