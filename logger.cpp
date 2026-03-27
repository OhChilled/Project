#include "logger.h"

#include <cstdlib>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>

namespace {

spdlog::level::level_enum parseLogLevel(const std::string &level) {
    if (level == "trace") {
        return spdlog::level::trace;
    }
    if (level == "debug") {
        return spdlog::level::debug;
    }
    if (level == "info") {
        return spdlog::level::info;
    }
    if (level == "warning" || level == "warn") {
        return spdlog::level::warn;
    }
    if (level == "error") {
        return spdlog::level::err;
    }
    if (level == "critical") {
        return spdlog::level::critical;
    }
    return spdlog::level::info;
}

} // namespace

namespace logging {

void initLogger(const std::string &level) {
    auto logger = spdlog::stdout_color_mt("cosmos");
    spdlog::set_default_logger(logger);

    // [2026-03-27 12:34:56.123] [INFO] [cosmos] message
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
    spdlog::set_level(parseLogLevel(level));
    spdlog::flush_on(spdlog::level::err);
}

std::string resolveLogLevel(int argc, char *argv[]) {
    if (argc > 1) {
        return argv[1];
    }

    const char *envLevel = std::getenv("APP_LOG_LEVEL");
    if (envLevel != nullptr) {
        return std::string(envLevel);
    }

#ifndef NDEBUG
    return "debug";
#else
    return "info";
#endif
}

} // namespace logging