@echo off
echo Formatting code...
clang-format -i *.cpp *.h

echo Running clang-tidy...
clang-tidy cosmos2025.cpp library.cpp -- -I.

echo Done.
pause