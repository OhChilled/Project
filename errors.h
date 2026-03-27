#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>
#include <utility>

class AppError : public std::runtime_error {
public:
    AppError(std::string errorId,
             std::string userMessage,
             std::string details)
        : std::runtime_error(std::move(details)),
          errorId_(std::move(errorId)),
          userMessage_(std::move(userMessage)) {}

    [[nodiscard]] const std::string &errorId() const {
        return errorId_;
    }

    [[nodiscard]] const std::string &userMessage() const {
        return userMessage_;
    }

private:
    std::string errorId_;
    std::string userMessage_;
};

#endif // ERRORS_H