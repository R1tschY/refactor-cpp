#ifndef LIB_REFACTOR_H_
#define LIB_REFACTOR_H_

#include "refactor-cpp-export.h"

namespace Refactor {

/// \todo rename to runRefactoringFromCmdln
/// \todo use llvm::ArrayRef<T>
REFACTOR_EXPORT int main(int argc, const char *argv[]);

} // namespace Refactor

#endif /* LIB_REFACTOR_H_ */
