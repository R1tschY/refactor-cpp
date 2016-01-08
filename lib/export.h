#ifndef LIB_EXPORT_H_
#define LIB_EXPORT_H_

/// \brief mark a function to be exported from library
#define REFACTOR_EXPORT __attribute__((visibility("default")))

#endif /* LIB_EXPORT_H_ */
