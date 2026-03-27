#include "logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace logging {

void initLogger() {
    auto logger = spdlog::stdout_color_mt("cosmos");
    spdlog::set_default_logger(logger);

    // [2026-03-27 12:34:56.123] [INFO] [cosmos] message
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");

#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    spdlog::flush_on(spdlog::level::err);
}

} // namespace logging