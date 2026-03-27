#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>

enum class Language {
    Ukrainian,
    English
};

namespace localization {

Language resolveLanguage(int argc, char *argv[]);
std::string toString(Language language);

} // namespace localization

#endif // LOCALIZATION_H