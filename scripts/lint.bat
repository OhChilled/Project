@echo off
echo Running clang-tidy...
clang-tidy cosmos2025.cpp library.cpp -- -I.
pause