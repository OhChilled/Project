# Project Simulation

## Опис проєкту

Проєкт є бакалаврською роботою на тему **«Моделювання процесу зближення та стикування космічних апаратів»**.

Програма реалізує чисельне моделювання процесу зближення космічних апаратів, обчислює параметри відносного руху та дозволяє аналізувати умови стикування.

## 📁 Структура проєкту

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
│   └── linting.md
├── scripts/
│   ├── check.bat
│   └── lint.bat
└── .githooks/
└── pre-commit


## Вимоги до середовища розробки

Для запуску та розробки проєкту потрібні:

* Git
* CMake 3.20 або новіше
* C++ компілятор з підтримкою C++17
* IDE або редактор коду:

    * CLion, або
    * Visual Studio / VS Code, або
    * будь-який інший редактор з підтримкою C++

## ⚙️ Збірка проєкту

```bash
cmake -S . -B build
cmake --build build
```
▶️ Запуск

```bash
build\Debug\untitled2.exe
```

## 🧪 Статичний аналіз (clang-tidy)

```bash
clang-tidy cosmos2025.cpp library.cpp -- -I.
```

## 🔗 Git Hooks

У проєкті використовується pre-commit hook, який автоматично запускає перевірки перед комітом.

Налаштування:

git config core.hooksPath .githooks

## 🏗 Інтеграція у процес збірки

Лінтинг інтегровано в систему збірки CMake:

```bash
cmake --build build --target lint
cmake --build build --target check
```
## 🧠 Статична типізація

Мова програмування C++ є статично типізованою.

Контроль коректності забезпечується:

  • компілятором C++
  • суворими попередженнями (-Wall -Wextra)
  • інструментом clang-tidy

## Автор

Ткаченко Данііл
