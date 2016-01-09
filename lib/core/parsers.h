#ifndef LIB_CORE_PARSERS_H_
#define LIB_CORE_PARSERS_H_

#include <map>
#include <string>
#include <vector>

namespace Refactor {

struct FuncDef {
  std::string name;
};

struct CommandOptions {
//  std::map<std::string, std::string> options;
  std::vector<std::string> arguments;
};

namespace Parsers {

FuncDef parseFunctionDefinition(const std::string& def);

CommandOptions parseCommandOptions(const std::string& command);

bool isIdentifier(const std::string& value);

}

} // namespace Refactor

#endif /* LIB_CORE_PARSERS_H_ */
