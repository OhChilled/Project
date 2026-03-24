#include "library.h"

double fx(double omega, double ax, double y0) {
    return 2.0 * omega * y0 + ax;
}

double fy(double omega, double ay, double x0, double y1) {
    return -2.0 * omega * x0 + 3.0 * omega * omega * y1 + ay;
}

double calcR(double x1, double y1) {
    return std::sqrt(x1 * x1 + y1 * y1);
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

    double k2 = fx(p.omega, p.ax, s.y0 + h * r1 / 2.0);
    double g2 = s.x0 + h * k1 / 2.0;
    double r2 = fy(p.omega, p.ay, s.x0 + h * k1 / 2.0, s.y1 + h * w1 / 2.0);
    double w2 = s.y0 + h * r1 / 2.0;

    double k3 = fx(p.omega, p.ax, s.y0 + h * r2 / 2.0);
    double g3 = s.x0 + h * k2 / 2.0;
    double r3 = fy(p.omega, p.ay, s.x0 + h * k2 / 2.0, s.y1 + h * w2 / 2.0);
    double w3 = s.y0 + h * r2 / 2.0;

    double k4 = fx(p.omega, p.ax, s.y0 + h * r3);
    double g4 = s.x0 + h * k3;
    double r4 = fy(p.omega, p.ay, s.x0 + h * k3, s.y1 + h * w3);
    double w4 = s.y0 + h * r3;

    State out = s;
    out.y0 += h * (r1 + 2*r2 + 2*r3 + r4) / 6.0;
    out.y1 += h * (w1 + 2*w2 + 2*w3 + w4) / 6.0;
    out.x0 += h * (k1 + 2*k2 + 2*k3 + k4) / 6.0;
    out.x1 += h * (g1 + 2*g2 + 2*g3 + g4) / 6.0;
    return out;
}
