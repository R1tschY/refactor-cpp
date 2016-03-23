#ifndef LIB_REFACTOR_CPP_EXPORT_H_
#define LIB_REFACTOR_CPP_EXPORT_H_

/// \brief mark a function to be exported from library
/// \todo use cpp-utils's CPP_EXPORT
#define REFACTOR_EXPORT __attribute__((visibility("default")))

#endif /* LIB_REFACTOR_CPP_EXPORT_H_ */
