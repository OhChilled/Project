#include "library.h"
#include "logger.h"
#include "error_utils.h"
#include "errors.h"

#include <cstdio>
#include <exception>
#include <iostream>
#include <limits>
#include <spdlog/spdlog.h>
#include <string>
#include <windows.h>
#include <cmath>

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
    logging::initLogger(logLevel);

    spdlog::info("Програма моделювання запущена");
    spdlog::info("Встановлений мінімальний рівень логування: {}", logLevel);

    FILE *file = nullptr;
    double t = TIME_START;

    State state{
        .x1 = INITIAL_POSITION,
        .x0 = INITIAL_VELOCITY,
        .y1 = INITIAL_POSITION,
        .y0 = INITIAL_VELOCITY,
    };

    try {
        const Params params{
            .omega = OMEGA,
            .ax = 0.0,
            .ay = 0.0,
            .h = TIME_STEP,
            .Rcrit = DOCKING_RADIUS,
        };

        Params runtimeParams = params;

        spdlog::info("Початок введення параметрів керування");
        std::cout << "Введіть ax, ay: ";

        if (!(std::cin >> runtimeParams.ax >> runtimeParams.ay)) {
            const std::string errorId = errors::generateErrorId();
            throw AppError(
                errorId,
                "Не вдалося зчитати параметри керування.",
                "Помилка введення параметрів ax та ay");
        }

        if (runtimeParams.h <= 0.0) {
            const std::string errorId = errors::generateErrorId();
            throw AppError(
                errorId,
                "Некоректний крок моделювання.",
                "Параметр h має бути більшим за 0");
        }

        if (runtimeParams.Rcrit <= 0.0) {
            const std::string errorId = errors::generateErrorId();
            throw AppError(
                errorId,
                "Некоректний радіус стикування.",
                "Параметр Rcrit має бути більшим за 0");
        }

        spdlog::info(
            "Параметри керування зчитано: {}",
            errors::buildParamsContext(runtimeParams));

        fopen_s(&file, "cosm2425.dan", "w");
        if (file == nullptr) {
            const std::string errorId = errors::generateErrorId();
            throw AppError(
                errorId,
                "Не вдалося відкрити файл результатів.",
                "Не вдалося відкрити файл cosm2425.dan для запису");
        }

        spdlog::info("Файл результатів cosm2425.dan успішно відкрито");

        std::fprintf(file, "Параметри моделювання:\n");
        std::fprintf(file, "omega = %lf, ax = %lf, ay = %lf\n",
                     runtimeParams.omega,
                     runtimeParams.ax,
                     runtimeParams.ay);
        std::fprintf(file, "Початкові умови: x1=%lf, x0=%lf, y1=%lf, y0=%lf\n",
                     state.x1,
                     state.x0,
                     state.y1,
                     state.y0);
        std::fprintf(file, "Крок h=%lf, Кінцевий час tk=%lf\n\n",
                     runtimeParams.h,
                     TIME_END);
        std::fprintf(file, "t\tx1\tx0\ty1\ty0\tR\n");

        spdlog::info("Початок чисельного моделювання");

        bool docked = false;

        while (t + runtimeParams.h <= TIME_END) {
            state = rk4Step(state, runtimeParams);
            t += runtimeParams.h;

            if (!std::isfinite(state.x1) || !std::isfinite(state.x0) ||
                !std::isfinite(state.y1) || !std::isfinite(state.y0)) {
                const std::string errorId = errors::generateErrorId();
                throw AppError(
                    errorId,
                    "Під час моделювання виникла чисельна помилка.",
                    "Стан системи містить нечислове або нескінченне значення");
            }

            const double distance = calcR(state.x1, state.y1);

            if (!std::isfinite(distance)) {
                const std::string errorId = errors::generateErrorId();
                throw AppError(
                    errorId,
                    "Під час обчислення відстані сталася помилка.",
                    "Обчислена відстань R не є коректним числом");
            }

            std::fprintf(file,
                         "%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\n",
                         t,
                         state.x1,
                         state.x0,
                         state.y1,
                         state.y0,
                         distance);

            spdlog::debug(
                "Крок моделювання: {}, R={:.3f}",
                errors::buildStateContext(state, t),
                distance);

            if (shouldDock(distance, runtimeParams.Rcrit)) {
                docked = true;
                spdlog::info(
                    "Стикування відбулося: {}, R={:.3f}",
                    errors::buildStateContext(state, t),
                    distance);
                std::printf("Стикування відбулося в момент t = %.3lf с\n", t);
                std::fprintf(file, "\nСтикування відбулося в момент t = %.3lf с\n", t);
                break;
            }
        }

        if (!docked) {
            const double finalDistance = calcR(state.x1, state.y1);
            spdlog::warn(
                "Стикування не відбулося. Контекст: {}, R={:.3f}",
                errors::buildStateContext(state, t),
                finalDistance);
            std::printf("Стикування не відбулося. Кінцева відстань: %.3lf м\n", finalDistance);
            std::fprintf(file, "\nСтикування не відбулося. Кінцева відстань: %.3lf м\n", finalDistance);
        }

        std::fclose(file);
        file = nullptr;

        spdlog::info("Результати збережено у файлі cosm2425.dan");
        spdlog::info("Програма завершила роботу успішно");
        return 0;
    } catch (const AppError &e) {
        spdlog::error(
            "AppError [{}]: {}. Деталі: {}. Параметри: {}. Стан: {}",
            e.errorId(),
            e.userMessage(),
            e.what(),
            "runtime parameters unavailable or partially initialized",
            errors::buildStateContext(state, t));

        if (file != nullptr) {
            std::fclose(file);
        }

        std::cerr << "Помилка: " << e.userMessage() << '\n'
                  << "Код помилки: " << e.errorId() << '\n';
        return 2;
    } catch (const std::exception &e) {
        const std::string errorId = errors::generateErrorId();

        spdlog::critical(
            "std::exception [{}]: {}. Стан: {}",
            errorId,
            e.what(),
            errors::buildStateContext(state, t));

        if (file != nullptr) {
            std::fclose(file);
        }

        std::cerr << "Критична помилка. Код помилки: " << errorId << '\n';
        return 3;
    } catch (...) {
        const std::string errorId = errors::generateErrorId();

        spdlog::critical(
            "Невідоме критичне виключення [{}]. Стан: {}",
            errorId,
            errors::buildStateContext(state, t));

        if (file != nullptr) {
            std::fclose(file);
        }

        std::cerr << "Невідома критична помилка. Код помилки: " << errorId << '\n';
        return 4;
    }
}