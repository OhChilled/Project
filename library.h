#ifndef LIBRARY_H
#define LIBRARY_H

#include <cmath>

/**
 * @brief Параметри моделі руху.
 */
struct Params {
    double omega; ///< Кутова швидкість.
    double ax;    ///< Керуюче прискорення по осі X.
    double ay;    ///< Керуюче прискорення по осі Y.
    double h;     ///< Крок інтегрування.
    double Rcrit; ///< Критична відстань стикування.
};

/**
 * @brief Поточний стан системи.
 */
struct State {
    double x1; ///< Координата X.
    double x0; ///< Швидкість по X.
    double y1; ///< Координата Y.
    double y0; ///< Швидкість по Y.
};

/**
 * @brief Обчислює похідну для руху по осі X.
 * @param omega Кутова швидкість.
 * @param ax Керуюче прискорення по осі X.
 * @param y0 Поточна швидкість по осі Y.
 * @return Значення похідної для осі X.
 */

// Модель (R1.1–R1.2)
double fx(double omega, double ax, double y0);

/**
 * @brief Обчислює похідну для руху по осі Y.
 * @param omega Кутова швидкість.
 * @param ax Керуюче прискорення по осі X.
 * @param y0 Поточна швидкість по осі Y.
 * @return Значення похідної для осі X.
 */

double fy(double omega, double ay, double x0, double y1);

/**
 * @brief Обчислює відстань до цілі.
 * @param x1 Поточна координата X.
 * @param y1 Поточна координата Y.
 * @return Евклідова відстань до цілі.
 */

// Відстань (R1.5)
double calcR(double x1, double y1);

/**
 * @brief Перевіряє умову стикування.
 * @param R Поточна відстань до цілі.
 * @param Rcrit Критична відстань стикування.
 * @return true, якщо стикування можливе, інакше false.
 */

// Критерій стикування (R1.6)
bool shouldDock(double R, double Rcrit);

/**
 * @brief Виконує один крок інтегрування методом Рунге-Кутти 4-го порядку.
 * @param s Поточний стан системи.
 * @param p Параметри моделі.
 * @return Новий стан системи після одного кроку інтегрування.
 */

// 1 крок RK4 (R1.2)
State rk4Step(const State &s, const Params &p);

#endif // LIBRARY_H