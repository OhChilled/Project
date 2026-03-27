#include "reporting.h"

#include "error_utils.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

namespace reporting {

void saveErrorReport(const std::string &errorId,
                     const std::string &technicalDetails,
                     const std::string &logLevel,
                     Language language,
                     const ExecutionContext &context) {
    std::filesystem::create_directories("reports");

    const std::string fileName = "reports/error_report_" + errorId + ".txt";
    std::ofstream out(fileName);

    if (!out.is_open()) {
        return;
    }

    const auto now = std::chrono::system_clock::now();
    const auto nowTimeT = std::chrono::system_clock::to_time_t(now);

    out << "==== ERROR REPORT ====\n";
    out << "Timestamp: " << std::put_time(std::localtime(&nowTimeT), "%Y-%m-%d %H:%M:%S") << '\n';
    out << "Operation ID: " << context.operationId << '\n';
    out << "Error ID: " << errorId << '\n';
    out << "Language: " << (language == Language::English ? "en" : "uk") << '\n';
    out << "Log level: " << logLevel << '\n';
    out << "Stage: " << context.stage << '\n';
    out << "Parameters: " << errors::buildParamsContext(context.runtimeParams) << '\n';
    out << "State: " << errors::buildStateContext(context.currentState, context.currentTime) << '\n';
    out << "Technical details: " << technicalDetails << '\n';
    out << "Log file: logs/cosmos.log\n";
    out << '\n';
    out << "Steps to reproduce:\n";
    out << "1. Describe what the user did before the error.\n";
    out << "2. Describe the entered parameters.\n";
    out << "3. Attach logs/cosmos.log if needed.\n";
    out << '\n';
    out << "User system information:\n";
    out << "- Fill in OS / environment manually if needed.\n";
}

} // namespace reporting