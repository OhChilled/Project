#include "error_utils.h"
#include "errors.h"
#include "execution_context.h"
#include "library.h"
#include "localization.h"
#include "logger.h"
#include "user_messages.h"

#include <cmath>
#include <cstdio>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <windows.h>

constexpr double OMEGA = 0.001;
constexpr double INITIAL_VELOCITY = 0.01;
constexpr double INITIAL_POSITION = 10.0;
constexpr double TIME_START = 0.0;
constexpr double TIME_STEP = 0.01;
constexpr double TIME_END = 7.0;
constexpr double DOCKING_RADIUS = 1.0;

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const std::string logLevel = logging::resolveLogLevel(argc, argv);
    const Language language = localization::resolveLanguage(argc, argv);

    logging::initLogger(logLevel);

    spdlog::info("Програма моделювання запущена");
    spdlog::info("Встановлений мінімальний рівень логування: {}", logLevel);
    spdlog::info("Вибрана мова інтерфейсу: {}", localization::toString(language));

    FILE *file = nullptr;

    ExecutionContext context{
        .runtimeParams = Params{
            .omega = OMEGA,
            .ax = 0.0,
            .ay = 0.0,
            .h = TIME_STEP,
            .Rcrit = DOCKING_RADIUS,
        },
        .currentState = State{
            .x1 = INITIAL_POSITION,
            .x0 = INITIAL_VELOCITY,
            .y1 = INITIAL_POSITION,
            .y0 = INITIAL_VELOCITY,
        },
        .currentTime = TIME_START,
        .stage = "startup",
        .paramsInitialized = false,
    };

    try {
        context.stage = "input";
        spdlog::info("Початок введення параметрів керування");
        std::cout << usermsg::getInputPrompt(language);

        if (!(std::cin >> context.runtimeParams.ax >> context.runtimeParams.ay)) {
            throw AppError(
                errors::generateErrorId(),
                usermsg::getInvalidInputMessage(language),
                "Помилка введення параметрів ax та ay");
        }

        context.paramsInitialized = true;

        if (context.runtimeParams.h <= 0.0) {
            throw AppError(
                errors::generateErrorId(),
                usermsg::getInvalidStepMessage(language),
                "Параметр h має бути більшим за 0");
        }

        if (context.runtimeParams.Rcrit <= 0.0) {
            throw AppError(
                errors::generateErrorId(),
                usermsg::getInvalidDockingRadiusMessage(language),
                "Параметр Rcrit має бути більшим за 0");
        }

        spdlog::info("Параметри ініціалізовано: {}",
                     errors::buildParamsContext(context.runtimeParams));

        context.stage = "open_output_file";
        fopen_s(&file, "cosm2425.dan", "w");
        if (file == nullptr) {
            throw AppError(
                errors::generateErrorId(),
                usermsg::getOutputFileOpenMessage(language),
                "Не вдалося відкрити файл cosm2425.dan для запису");
        }

        spdlog::info("Файл результатів cosm2425.dan успішно відкрито");
        spdlog::info("Логи також записуються у файл logs/cosmos.log");

        std::fprintf(file, "Параметри моделювання:\n");
        std::fprintf(file, "omega = %lf, ax = %lf, ay = %lf\n",
                     context.runtimeParams.omega,
                     context.runtimeParams.ax,
                     context.runtimeParams.ay);
        std::fprintf(file, "Початкові умови: x1=%lf, x0=%lf, y1=%lf, y0=%lf\n",
                     context.currentState.x1,
                     context.currentState.x0,
                     context.currentState.y1,
                     context.currentState.y0);
        std::fprintf(file, "Крок h=%lf, Кінцевий час tk=%lf\n\n",
                     context.runtimeParams.h,
                     TIME_END);
        std::fprintf(file, "t\tx1\tx0\ty1\ty0\tR\n");

        context.stage = "simulation";
        spdlog::info("Початок чисельного моделювання");

        bool docked = false;

        while (context.currentTime + context.runtimeParams.h <= TIME_END) {
            context.currentState = rk4Step(context.currentState, context.runtimeParams);
            context.currentTime += context.runtimeParams.h;

            if (!std::isfinite(context.currentState.x1) ||
                !std::isfinite(context.currentState.x0) ||
                !std::isfinite(context.currentState.y1) ||
                !std::isfinite(context.currentState.y0)) {
                throw AppError(
                    errors::generateErrorId(),
                    usermsg::getNumericFailureMessage(language),
                    "Стан системи містить нечислове або нескінченне значення");
            }

            const double distance = calcR(context.currentState.x1, context.currentState.y1);

            if (!std::isfinite(distance)) {
                throw AppError(
                    errors::generateErrorId(),
                    usermsg::getDistanceFailureMessage(language),
                    "Обчислена відстань R не є коректним числом");
            }

            std::fprintf(file,
                         "%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\n",
                         context.currentTime,
                         context.currentState.x1,
                         context.currentState.x0,
                         context.currentState.y1,
                         context.currentState.y0,
                         distance);

            spdlog::debug(
                "Крок моделювання: {}; R={:.3f}",
                errors::buildExecutionContext(
                    context.stage,
                    context.runtimeParams,
                    context.currentState,
                    context.currentTime),
                distance);

            if (shouldDock(distance, context.runtimeParams.Rcrit)) {
                docked = true;
                context.stage = "docking";
                spdlog::info(
                    "Стикування відбулося: {}; R={:.3f}",
                    errors::buildExecutionContext(
                        context.stage,
                        context.runtimeParams,
                        context.currentState,
                        context.currentTime),
                    distance);
                const std::string dockingMessage =
                    usermsg::getDockingSuccessMessage(language, context.currentTime);
                std::cout << dockingMessage << '\n';
                std::fprintf(file, "\n%s\n", dockingMessage.c_str());
                break;
            }
        }

        if (!docked) {
            context.stage = "finish_without_docking";
            const double finalDistance = calcR(context.currentState.x1, context.currentState.y1);
            spdlog::warn(
                "Стикування не відбулося: {}; R={:.3f}",
                errors::buildExecutionContext(
                    context.stage,
                    context.runtimeParams,
                    context.currentState,
                    context.currentTime),
                finalDistance);
            const std::string failureMessage =
                usermsg::getDockingFailureMessage(language, finalDistance);
            std::cout << failureMessage << '\n';
            std::fprintf(file, "\n%s\n", failureMessage.c_str());
        }

        context.stage = "shutdown";

        std::fclose(file);
        file = nullptr;

        spdlog::info("Результати збережено у файлі cosm2425.dan");
        spdlog::info("Програма завершила роботу успішно");
        return 0;
    } catch (const AppError &e) {
        spdlog::error(
            "AppError [{}]: {}. Деталі: {}. Контекст: {}",
            e.errorId(),
            e.userMessage(),
            e.what(),
            errors::buildExecutionContext(
                context.stage,
                context.runtimeParams,
                context.currentState,
                context.currentTime));

        if (file != nullptr) {
            std::fclose(file);
        }

        std::cerr << usermsg::buildFullUserErrorMessage(
                         language,
                         e.userMessage(),
                         e.errorId())
                  << '\n';
        return 2;
    } catch (const std::exception &e) {
        const std::string errorId = errors::generateErrorId();

        spdlog::critical(
            "std::exception [{}]: {}. Контекст: {}",
            errorId,
            e.what(),
            errors::buildExecutionContext(
                context.stage,
                context.runtimeParams,
                context.currentState,
                context.currentTime));

        if (file != nullptr) {
            std::fclose(file);
        }

        std::cerr << usermsg::buildFullUserErrorMessage(
                         language,
                         usermsg::getUnknownFailureMessage(language),
                         errorId)
                  << '\n';
        return 3;
    } catch (...) {
        const std::string errorId = errors::generateErrorId();

        spdlog::critical(
            "Невідоме критичне виключення [{}]. Контекст: {}",
            errorId,
            errors::buildExecutionContext(
                context.stage,
                context.runtimeParams,
                context.currentState,
                context.currentTime));

        if (file != nullptr) {
            std::fclose(file);
        }

        std::cerr << usermsg::buildFullUserErrorMessage(
                         language,
                         usermsg::getUnknownFailureMessage(language),
                         errorId)
                  << '\n';
        return 4;
    }
}