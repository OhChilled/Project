#include "localization.h"

#include <cstdlib>
#include <string>

namespace localization {

Language resolveLanguage(int argc, char *argv[]) {
    if (argc > 2) {
        const std::string arg = argv[2];
        if (arg == "en") {
            return Language::English;
        }
        if (arg == "uk") {
            return Language::Ukrainian;
        }
    }

    const char *envLang = std::getenv("APP_LANG");
    if (envLang != nullptr) {
        const std::string lang = envLang;
        if (lang == "en") {
            return Language::English;
        }
    }

    return Language::Ukrainian;
}

std::string toString(Language language) {
    if (language == Language::English) {
        return "en";
    }
    return "uk";
}

} // namespace localization