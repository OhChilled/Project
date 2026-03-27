#ifndef REPORTING_H
#define REPORTING_H

#include "execution_context.h"
#include "localization.h"

#include <string>

namespace reporting {

void saveErrorReport(const std::string &errorId,
                     const std::string &technicalDetails,
                     const std::string &logLevel,
                     Language language,
                     const ExecutionContext &context);

} // namespace reporting

#endif // REPORTING_H