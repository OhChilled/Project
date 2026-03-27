#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace logging {
void initLogger(const std::string &level);
std::string resolveLogLevel(int argc, char *argv[]);
} // namespace logging

#endif // LOGGER_H