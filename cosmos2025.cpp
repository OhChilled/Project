#include "library.h"
#include "logger.h"

#include <cstdio>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>
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
    logging::initLogger(logLevel);

    spdlog::info("Програма моделювання запущена");
    spdlog::info("Встановлений мінімальний рівень логування: {}", logLevel);

    FILE *file = nullptr;

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
            spdlog::error("Не вдалося зчитати параметри ax та ay");
            std::cerr << "Помилка введення параметрів ax та ay.\n";
            return 1;
        }

        spdlog::info("Параметри керування зчитано: omega={}, ax={}, ay={}, h={}, Rcrit={}",
                     runtimeParams.omega, runtimeParams.ax, runtimeParams.ay, runtimeParams.h,
                     runtimeParams.Rcrit);

        fopen_s(&file, "cosm2425.dan", "w");
        if (file == nullptr) {
            spdlog::critical("Не вдалося відкрити файл cosm2425.dan для запису");
            std::cerr << "Помилка відкриття файлу cosm2425.dan.\n";
            return 1;
        }

        spdlog::info("Файл результатів cosm2425.dan успішно відкрито");

        State state{
            .x1 = INITIAL_POSITION,
            .x0 = INITIAL_VELOCITY,
            .y1 = INITIAL_POSITION,
            .y0 = INITIAL_VELOCITY,
        };

        double t = TIME_START;

        std::fprintf(file, "Параметри моделювання:\n");
        std::fprintf(file, "omega = %lf, ax = %lf, ay = %lf\n", runtimeParams.omega,
                     runtimeParams.ax, runtimeParams.ay);
        std::fprintf(file, "Початкові умови: x1=%lf, x0=%lf, y1=%lf, y0=%lf\n", state.x1, state.x0,
                     state.y1, state.y0);
        std::fprintf(file, "Крок h=%lf, Кінцевий час tk=%lf\n\n", runtimeParams.h, TIME_END);
        std::fprintf(file, "t\tx1\tx0\ty1\ty0\tR\n");

        spdlog::info("Початок чисельного моделювання");

        bool docked = false;

        while (t + runtimeParams.h <= TIME_END) {
            state = rk4Step(state, runtimeParams);
            t += runtimeParams.h;

            const double distance = calcR(state.x1, state.y1);

            std::fprintf(file, "%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\n", t, state.x1, state.x0,
                         state.y1, state.y0, distance);

            spdlog::debug(
                "Крок моделювання: t={:.3f}, x1={:.3f}, x0={:.3f}, y1={:.3f}, y0={:.3f}, R={:.3f}",
                t, state.x1, state.x0, state.y1, state.y0, distance);

            if (shouldDock(distance, runtimeParams.Rcrit)) {
                docked = true;
                spdlog::info("Стикування відбулося в момент t={:.3f} c, R={:.3f}", t, distance);
                std::printf("Стикування відбулося в момент t = %.3lf с\n", t);
                std::fprintf(file, "\nСтикування відбулося в момент t = %.3lf с\n", t);
                break;
            }
        }

        if (!docked) {
            const double finalDistance = calcR(state.x1, state.y1);
            spdlog::warn("Стикування не відбулося. Кінцева відстань R={:.3f}", finalDistance);
            std::printf("Стикування не відбулося. Кінцева відстань: %.3lf м\n", finalDistance);
            std::fprintf(file, "\nСтикування не відбулося. Кінцева відстань: %.3lf м\n",
                         finalDistance);
        }

        std::fclose(file);
        file = nullptr;

        spdlog::info("Результати збережено у файлі cosm2425.dan");
        spdlog::info("Програма завершила роботу успішно");
        return 0;
    } catch (const std::exception &e) {
        spdlog::critical("Виключення std::exception: {}", e.what());
        if (file != nullptr) {
            std::fclose(file);
        }
        std::cerr << "Критична помилка: " << e.what() << '\n';
        return 2;
    } catch (...) {
        spdlog::critical("Невідоме критичне виключення");
        if (file != nullptr) {
            std::fclose(file);
        }
        std::cerr << "Невідома критична помилка.\n";
        return 3;
    }
}