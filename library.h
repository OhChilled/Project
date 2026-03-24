#pragma once
#include <cmath>

struct Params {
    double omega;
    double ax;
    double ay;
    double h;
    double Rcrit;
};

struct State {
    double x1; // координата x
    double x0; // швидкість x
    double y1; // координата y
    double y0; // швидкість y
};

// Модель (R1.1–R1.2)
double fx(double omega, double ax, double y0);
double fy(double omega, double ay, double x0, double y1);

// Відстань (R1.5)
double calcR(double x1, double y1);

// Критерій стикування (R1.6)
bool shouldDock(double R, double Rcrit);

// 1 крок RK4 (R1.2)
State rk4Step(const State& s, const Params& p);
