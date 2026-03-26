#include "library.h"

namespace {
constexpr double CORIOLIS_FACTOR = 2.0;
constexpr double GRAVITY_FACTOR = 3.0;
constexpr double HALF = 2.0;
constexpr double RK_DIVISOR = 6.0;
constexpr double RK_WEIGHT = 2.0;
}

double fx(double omega, double ax, double y0) {
    return (CORIOLIS_FACTOR * omega * y0) + ax;
}

double fy(double omega, double ay, double x0, double y1) {
    return -(CORIOLIS_FACTOR * omega * x0) + (GRAVITY_FACTOR * omega * omega * y1) + ay;
}

double calcR(double x1, double y1) {
    return std::sqrt((x1 * x1) + (y1 * y1));
}

bool shouldDock(double R, double Rcrit) {
    return R <= Rcrit;
}

State rk4Step(const State& s, const Params& p) {
    const double h = p.h;

    double k1 = fx(p.omega, p.ax, s.y0);
    double g1 = s.x0;
    double r1 = fy(p.omega, p.ay, s.x0, s.y1);
    double w1 = s.y0;

    double k2 = fx(p.omega, p.ax, s.y0 + ((h * r1) / HALF));
    double g2 = s.x0 + ((h * k1) / HALF);
    double r2 = fy(p.omega, p.ay, s.x0 + ((h * k1) / HALF), s.y1 + ((h * w1) / HALF));
    double w2 = s.y0 + ((h * r1) / HALF);

    double k3 = fx(p.omega, p.ax, s.y0 + ((h * r2) / HALF));
    double g3 = s.x0 + ((h * k2) / HALF);
    double r3 = fy(p.omega, p.ay, s.x0 + ((h * k2) / HALF), s.y1 + ((h * w2) / HALF));
    double w3 = s.y0 + ((h * r2) / HALF);

    double k4 = fx(p.omega, p.ax, s.y0 + (h * r3));
    double g4 = s.x0 + (h * k3);
    double r4 = fy(p.omega, p.ay, s.x0 + (h * k3), s.y1 + (h * w3));
    double w4 = s.y0 + (h * r3);

    State out = s;
    out.y0 += (h * (r1 + (RK_WEIGHT * r2) + (RK_WEIGHT * r3) + r4)) / RK_DIVISOR;
    out.y1 += (h * (w1 + (RK_WEIGHT * w2) + (RK_WEIGHT * w3) + w4)) / RK_DIVISOR;
    out.x0 += (h * (k1 + (RK_WEIGHT * k2) + (RK_WEIGHT * k3) + k4)) / RK_DIVISOR;
    out.x1 += (h * (g1 + (RK_WEIGHT * g2) + (RK_WEIGHT * g3) + g4)) / RK_DIVISOR;
    return out;
}
