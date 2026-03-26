# 🧪 Static Code Analysis (Linting)

This project uses **clang-tidy** for static code analysis.

---

## 🔍 Applied Checks

- `readability-magic-numbers` — detection of "magic numbers"
- `readability-isolate-declaration` — separation of multiple variable declarations
- `readability-math-missing-parentheses` — checking operator precedence in expressions

---

## ⚙️ Running the Check

To check the code, use the command:

```bash
clang-tidy cosmos2025.cpp library.cpp -- -I.
```

For formatting:

```bash
clang-format -i *.cpp *.h
```

For a comprehensive check:

```bash
scripts/check.bat
```