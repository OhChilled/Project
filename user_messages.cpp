#include "user_messages.h"

#include <sstream>
#include <string>

namespace usermsg {

std::string getInvalidInputMessage(Language language) {
    if (language == Language::English) {
        return "Failed to read control parameters from input.";
    }
    return "Не вдалося зчитати параметри керування з введення.";
}

std::string getInvalidStepMessage(Language language) {
    if (language == Language::English) {
        return "The simulation step is invalid.";
    }
    return "Крок моделювання задано некоректно.";
}

std::string getInvalidDockingRadiusMessage(Language language) {
    if (language == Language::English) {
        return "The docking radius is invalid.";
    }
    return "Радіус стикування задано некоректно.";
}

std::string getOutputFileOpenMessage(Language language) {
    if (language == Language::English) {
        return "Could not create the output file.";
    }
    return "Не вдалося створити файл результатів.";
}

std::string getNumericFailureMessage(Language language) {
    if (language == Language::English) {
        return "A numerical error occurred during simulation.";
    }
    return "Під час моделювання виникла чисельна помилка.";
}

std::string getDistanceFailureMessage(Language language) {
    if (language == Language::English) {
        return "Failed to compute the distance correctly.";
    }
    return "Не вдалося коректно обчислити відстань.";
}

std::string getUnknownFailureMessage(Language language) {
    if (language == Language::English) {
        return "An unexpected critical error occurred.";
    }
    return "Сталася неочікувана критична помилка.";
}

std::string getActionInstructions(Language language) {
    if (language == Language::English) {
        return "Please check the input data, restart the program, and try again.";
    }
    return "Перевірте введені дані, перезапустіть програму та спробуйте ще раз.";
}

std::string getLogLocationMessage(Language language) {
    if (language == Language::English) {
        return "Technical details are saved in logs/cosmos.log.";
    }
    return "Технічні деталі збережено у файлі logs/cosmos.log.";
}

std::string getReportProblemMessage(Language language, const std::string &errorId) {
    std::ostringstream oss;
    if (language == Language::English) {
        oss << "If the problem persists, send the error code " << errorId
            << " and the log file to the developer.";
    } else {
        oss << "Якщо проблема повториться, передайте розробнику код помилки "
            << errorId << " та файл логів.";
    }
    return oss.str();
}

std::string buildFullUserErrorMessage(Language language,
                                      const std::string &mainMessage,
                                      const std::string &errorId) {
    std::ostringstream oss;
    oss << mainMessage << '\n'
        << getActionInstructions(language) << '\n'
        << getLogLocationMessage(language) << '\n'
        << getReportProblemMessage(language, errorId);
    return oss.str();
}

std::string getInputPrompt(Language language) {
    if (language == Language::English) {
        return "Enter ax, ay: ";
    }
    return "Введіть ax, ay: ";
}

std::string getDockingSuccessMessage(Language language, double time) {
    std::ostringstream oss;
    if (language == Language::English) {
        oss << "Docking occurred at t = " << time << " s";
    } else {
        oss << "Стикування відбулося в момент t = " << time << " с";
    }
    return oss.str();
}

std::string getDockingFailureMessage(Language language, double distance) {
    std::ostringstream oss;
    if (language == Language::English) {
        oss << "Docking did not occur. Final distance: " << distance << " m";
    } else {
        oss << "Стикування не відбулося. Кінцева відстань: " << distance << " м";
    }
    return oss.str();
}

std::string getOperationIdMessage(Language language, const std::string &operationId) {
    if (language == Language::English) {
        return "Operation ID: " + operationId;
    }
    return "Ідентифікатор операції: " + operationId;
}

} // namespace usermsg