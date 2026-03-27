#ifndef EXECUTION_CONTEXT_H
#define EXECUTION_CONTEXT_H

#include "library.h"

#include <string>

struct ExecutionContext {
    Params runtimeParams{};
    State currentState{};
    double currentTime{0.0};
    std::string stage{"startup"};
    bool paramsInitialized{false};
};

#endif // EXECUTION_CONTEXT_H