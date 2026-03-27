#ifndef ERROR_UTILS_H
#define ERROR_UTILS_H

#include "library.h"

#include <string>

namespace errors {

std::string generateErrorId();
std::string buildStateContext(const State &state, double time);
std::string buildParamsContext(const Params &params);

} // namespace errors

#endif // ERROR_UTILS_H