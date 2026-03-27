#include "error_utils.h"

#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

namespace errors {

std::string generateErrorId() {
    const auto now = std::chrono::system_clock::now().time_since_epoch().count();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1000, 9999);

    std::ostringstream oss;
    oss << "ERR-" << now << "-" << dist(gen);
    return oss.str();
}

std::string buildStateContext(const State &state, double time) {
    std::ostringstream oss;
    oss << "t=" << std::fixed << std::setprecision(3) << time
        << ", x1=" << state.x1
        << ", x0=" << state.x0
        << ", y1=" << state.y1
        << ", y0=" << state.y0;
    return oss.str();
}

std::string buildParamsContext(const Params &params) {
    std::ostringstream oss;
    oss << "omega=" << params.omega
        << ", ax=" << params.ax
        << ", ay=" << params.ay
        << ", h=" << params.h
        << ", Rcrit=" << params.Rcrit;
    return oss.str();
}

std::string buildExecutionContext(const std::string &stage,
                                  const Params &params,
                                  const State &state,
                                  double time) {
    std::ostringstream oss;
    oss << "stage=" << stage
        << "; params={" << buildParamsContext(params) << "}"
        << "; state={" << buildStateContext(state, time) << "}";
    return oss.str();
}

} // namespace errors