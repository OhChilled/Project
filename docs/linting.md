# Linting

У проєкті використовується:
- clang-tidy
- clang-format
- cppcheck

## Запуск

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
clang-tidy cosmos2025.cpp library.cpp -- -I.

## Примітка

Основна математична логіка проєкту в межах лабораторної роботи не змінювалась.
Основна увага приділена конфігурації лінтингу, документації, збірці та автоматизації перевірок.