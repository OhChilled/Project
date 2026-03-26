# Test-Driven Documentation
## 📌 General Idea

In this project, unit tests are used not only to check the correctness of the code but also as examples of how to use functions.

This approach is called Test-Driven Documentation.

## 📖 This means that:

tests demonstrate how to call functions
show expected results
serve as "live" documentation

## 🧩 Examples from the project
🔹 Function fx
TEST(ModelRequirements, FxMatchesFormula_R11_R12) {
double omega = 0.001, ax = 1.0, y0 = 10.0;
double expected = 2.0 * omega * y0 + ax;
ASSERT_NEAR(fx(omega, ax, y0), expected, 1e-12);
}

Shows:

how to call fx
which parameters to pass
that the function returns a value according to the formula
🔹 Function fy
TEST(ModelRequirements, FyMatchesFormula_R11_R12) {
double omega = 0.001, ay = 2.0, x0 = 0.01, y1 = 10.0;
double expected = -2.0 * omega * x0 + 3.0 * omega * omega * y1 + ay;
ASSERT_NEAR(fy(omega, ay, x0, y1), expected, 1e-12);
}

Demonstrates the use of the formula for fy

🔹 Function calcR
TEST(ModelRequirements, DistanceIsSqrtAndNonNegative_R15) {
double R = calcR(-3.0, 4.0);
ASSERT_NEAR(R, 5.0, 1e-12);
ASSERT_GE(R, 0.0);
}

Shows:

distance calculation
that the result is always non-negative

🔹 Function shouldDock
TEST(ModelRequirements, DockingCriterionInclusive_R16) {
ASSERT_TRUE(shouldDock(1.0, 1.0));
ASSERT_TRUE(shouldDock(0.999, 1.0));
ASSERT_FALSE(shouldDock(1.001, 1.0));
}

Explains:
docking condition
boundary values

🔹 Function rk4Step
TEST(IntegratorRequirements, RK4ControlCase_Omega0_NoAccel_R12) {
Params p{0.0, 0.0, 0.0, 0.01, 1.0};
State s{10.0, 0.01, 10.0, 0.01};

State s2 = rk4Step(s, p);

ASSERT_NEAR(s2.x0, s.x0, 1e-12);
ASSERT_NEAR(s2.y0, s.y0, 1e-12);
ASSERT_NEAR(s2.x1, s.x1 + s.x0 * p.h, 1e-8);
ASSERT_NEAR(s2.y1, s.y1 + s.y0 * p.h, 1e-8);
}

Demonstrates:

example of using the Runge-Kutta method
control case without acceleration

## ▶️ How to run tests
ctest --test-dir build