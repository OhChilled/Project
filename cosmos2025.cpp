#include "error_utils.h"
#include "errors.h"
#include "execution_context.h"
#include "library.h"
#include "localization.h"
#include "logger.h"
#include "user_messages.h"
#include "reporting.h"

#include <chrono>
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
        .operationId = errors::generateOperationId(),
    };

    spdlog::info("Operation ID: {}", context.operationId);
    std::cout << usermsg::getOperationIdMessage(language, context.operationId) << '\n';

    const auto appStart = std::chrono::high_resolution_clock::now();
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

        std::fprintf(file, "Operation ID: %s\n", context.operationId.c_str());
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
        const auto simulationStart = std::chrono::high_resolution_clock::now();

        bool docked = false;

        long long rk4StepTimeNs = 0;
        long long calcRTimeNs = 0;
        long long shouldDockTimeNs = 0;
        int iterationCount = 0;

        while (context.currentTime + context.runtimeParams.h <= TIME_END) {
            const auto rk4Start = std::chrono::high_resolution_clock::now();
            context.currentState = rk4Step(context.currentState, context.runtimeParams);
            const auto rk4End = std::chrono::high_resolution_clock::now();
            rk4StepTimeNs += std::chrono::duration_cast<std::chrono::nanoseconds>(rk4End - rk4Start).count();
            context.currentTime += context.runtimeParams.h;
            ++iterationCount;

            if (!std::isfinite(context.currentState.x1) ||
                !std::isfinite(context.currentState.x0) ||
                !std::isfinite(context.currentState.y1) ||
                !std::isfinite(context.currentState.y0)) {
                throw AppError(
                    errors::generateErrorId(),
                    usermsg::getNumericFailureMessage(language),
                    "Стан системи містить нечислове або нескінченне значення");
            }

            const auto calcRStart = std::chrono::high_resolution_clock::now();
            const double distance = calcR(context.currentState.x1, context.currentState.y1);
            const auto calcREnd = std::chrono::high_resolution_clock::now();
            calcRTimeNs += std::chrono::duration_cast<std::chrono::nanoseconds>(calcREnd - calcRStart).count();

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

            if (iterationCount % 50 == 0) {
                spdlog::debug(
                    "Крок моделювання: {}; R={:.3f}",
                    errors::buildExecutionContext(
                        context.stage,
                        context.runtimeParams,
                        context.currentState,
                        context.currentTime),
                    distance);
            }

            const auto shouldDockStart = std::chrono::high_resolution_clock::now();
            const bool dockingNow = shouldDock(distance, context.runtimeParams.Rcrit);
            const auto shouldDockEnd = std::chrono::high_resolution_clock::now();
            shouldDockTimeNs += std::chrono::duration_cast<std::chrono::nanoseconds>(shouldDockEnd - shouldDockStart).count();

            if (dockingNow) {
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
            const auto calcRStart = std::chrono::high_resolution_clock::now();
            const double finalDistance = calcR(context.currentState.x1, context.currentState.y1);
            const auto calcREnd = std::chrono::high_resolution_clock::now();
            calcRTimeNs += std::chrono::duration_cast<std::chrono::nanoseconds>(calcREnd - calcRStart).count();
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

        const auto simulationEnd = std::chrono::high_resolution_clock::now();
        const auto simulationDuration =
            std::chrono::duration_cast<std::chrono::milliseconds>(simulationEnd - simulationStart);

        spdlog::info("Кількість ітерацій: {}", iterationCount);
        spdlog::info("Сумарний час rk4Step: {} ns", rk4StepTimeNs);
        spdlog::info("Сумарний час calcR: {} ns", calcRTimeNs);
        spdlog::info("Сумарний час shouldDock: {} ns", shouldDockTimeNs);

        std::cout << "Iterations: " << iterationCount << '\n';
        std::cout << "rk4Step total time: " << rk4StepTimeNs << " ns\n";
        std::cout << "calcR total time: " << calcRTimeNs << " ns\n";
        std::cout << "shouldDock total time: " << shouldDockTimeNs << " ns\n";

        spdlog::info("Час виконання моделювання: {} ms", simulationDuration.count());
        std::cout << "Simulation time: " << simulationDuration.count() << " ms\n";
        context.stage = "shutdown";

        std::fclose(file);
        file = nullptr;

        spdlog::info("Результати збережено у файлі cosm2425.dan");
        spdlog::info("Програма завершила роботу успішно");
        const auto appEnd = std::chrono::high_resolution_clock::now();
        const auto appDuration =
            std::chrono::duration_cast<std::chrono::milliseconds>(appEnd - appStart);

        spdlog::info(
            "PERF SUMMARY | operation_id={} | iterations={} | simulation_ms={} | total_ms={} | rk4_ns={} | calcR_ns={} | shouldDock_ns={}",
            context.operationId,
            iterationCount,
            simulationDuration.count(),
            appDuration.count(),
            rk4StepTimeNs,
            calcRTimeNs,
            shouldDockTimeNs);

        spdlog::info("Загальний час виконання програми: {} ms", appDuration.count());
        std::cout << "Total execution time: " << appDuration.count() << " ms\n";

        return 0;
    } catch (const AppError &e) {
        spdlog::error(
            "AppError [{}]: {}. Деталі: {}. Контекст: {}. Operation ID: {}",
            e.errorId(),
            e.userMessage(),
            e.what(),
            errors::buildExecutionContext(
                context.stage,
                context.runtimeParams,
                context.currentState,
                context.currentTime),
            context.operationId);

        if (file != nullptr) {
            std::fclose(file);
        }

        reporting::saveErrorReport(
            e.errorId(),
            e.userMessage(),
            logLevel,
            language,
            context);

        std::cerr << usermsg::buildFullUserErrorMessage(
                         language,
                         e.userMessage(),
                         e.errorId())
                  << '\n';
        std::cerr << usermsg::getReportSavedMessage(language, e.errorId()) << '\n';
        return 2;
    } catch (const std::exception &e) {
        const std::string errorId = errors::generateErrorId();

        spdlog::critical(
            "std::exception [{}]: {}. Контекст: {}. Operation ID: {}",
            errorId,
            e.what(),
            errors::buildExecutionContext(
                context.stage,
                context.runtimeParams,
                context.currentState,
                context.currentTime),
            context.operationId);

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
            "Невідоме критичне виключення [{}]. Контекст: {}. Operation ID: {}",
            errorId,
            errors::buildExecutionContext(
                context.stage,
                context.runtimeParams,
                context.currentState,
                context.currentTime),
            context.operationId);

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