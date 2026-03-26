# Project Simulation

## Project Description

The project is a bachelor's thesis on **"Modeling the Approach and Docking Process of Spacecraft"**.

The program implements numerical modeling of the spacecraft approach process, calculates relative motion parameters, and allows analyzing docking conditions.

## 🌍 Multilingual Documentation

The project supports documentation in two languages:

- Ukrainian: `docs/`
- English: `docs/en/`

## 📚 Online Documentation

The documentation is automatically generated using **Doxygen** and published via **GitHub Pages**.

## 🚀 Automatic Publishing

After a push to the `main` branch, GitHub Actions:

- generates HTML documentation
- publishes it on GitHub Pages

## 📁 Project Structure

Project/
├── cosmos2025.cpp
├── library.cpp
├── library.h
├── CMakeLists.txt
├── README.md
├── .clang-tidy
├── .clang-format
├── .gitignore
├── docs/
│ └── linting.md
├── scripts/
│ ├── check.bat
│ └── lint.bat
└── .githooks/
└── pre-commit

## Development Environment Requirements

To run and develop the project, you need:

* Git
* CMake 3.20 or newer
* C++ compiler with C++17 support
* IDE or code editor:
    * CLion, or
    * Visual Studio / VS Code, or
    * any other editor with C++ support

## ⚙️ Project Build

```bash
cmake -S . -B build
cmake --build build
```
▶️ Run

```bash
buildDebuguntitled2.exe
```

## 🧪 Static Analysis (clang-tidy)

```bash
clang-tidy cosmos2025.cpp library.cpp -- -I.
```

## 🔗 Git Hooks

The project uses a pre-commit hook that automatically runs checks before a commit.

Setup:

git config core.hooksPath .githooks

## 🏗 Integration into the Build Process

Linting is integrated into the CMake build system:

```bash
cmake --build build --target lint
cmake --build build --target check
```

## 🧠 Static Typing

The C++ programming language is statically typed.

Correctness is ensured by:

• C++ compiler
• strict warnings (-Wall -Wextra)
• clang-tidy tool

## 📝 Code Documentation

The project uses documentation in the Doxygen style.

All public functions, structures, and methods must include:

- a brief description (`@brief`)
- a description of parameters (`@param`)
- a description of the return value (`@return`), if the function returns something

Recommendations:
- document the purpose of the function simply and briefly
- explain the physical meaning of the parameters
- update documentation when code is changed

## Author

Tkachenko Daniil