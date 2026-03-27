#ifndef USER_MESSAGES_H
#define USER_MESSAGES_H

#include "localization.h"

#include <string>

namespace usermsg {

std::string getInvalidInputMessage(Language language);
std::string getInvalidStepMessage(Language language);
std::string getInvalidDockingRadiusMessage(Language language);
std::string getOutputFileOpenMessage(Language language);
std::string getNumericFailureMessage(Language language);
std::string getDistanceFailureMessage(Language language);
std::string getUnknownFailureMessage(Language language);

std::string getActionInstructions(Language language);
std::string getReportProblemMessage(Language language, const std::string &errorId);
std::string getLogLocationMessage(Language language);
std::string buildFullUserErrorMessage(Language language,
                                      const std::string &mainMessage,
                                      const std::string &errorId);
std::string getInputPrompt(Language language);
std::string getDockingSuccessMessage(Language language, double time);
std::string getDockingFailureMessage(Language language, double distance);

} // namespace usermsg

#endif // USER_MESSAGES_H