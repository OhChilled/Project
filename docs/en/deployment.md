# Project Deployment Instructions in the Production Environment

## 1. General Information
The project is a C++ console application designed to simulate the process of spacecraft rendezvous and docking.

The project does not use:
- web server
- application server
- DBMS
- caching services

Deployment consists of building and running the application on the target machine.

---

## 2. Project Architecture
The application structure includes:
- program launch module
- mathematical modeling modules
- testing module
- CMake build configuration

Workflow diagram:

User
↓
Console application
↓
Input parameter processing
↓
Mathematical modeling
↓
Results formation
↓
Output of results to console or file

---

## 3. Hardware Requirements
Minimum requirements:
- Architecture: x86_64
- CPU: 2 cores
- RAM: 4 GB
- Free disk space: 1 GB

Recommended requirements:
- CPU: 4 cores
- RAM: 8 GB
- SSD for faster build and execution

---

## 4. Required Software
The target machine must have installed:
- Git
- CMake 3.20 or newer
- A C++ compiler with C++17 support

### Linux
Recommended:
- Ubuntu 22.04+ or another modern distribution
- g++

### Windows
Recommended:
- Windows 10/11
- Visual Studio Build Tools or MinGW

---

## 5. Network Setup
For the basic operation of the project, special network settings are not required, as the project is a local console application.

Network connection is only needed for:
- cloning the repository
- getting updates
- downloading dependencies, if necessary

---

## 6. Server / Target Machine Configuration
On the target machine, it is necessary to:
1. Create a working directory for the project
2. Clone the repository
3. Create a `build` directory
4. Execute configuration and build

---

## 7. Code Deployment

### Step 1. Clone the Repository
```bash
git clone https://github.com/OhChilled/Project.git
cd untitled2
```
## Project Architecture

The project is a console application and does not use a web server, application server, DBMS, or caching.

Workflow diagram:

User
↓
Console application
↓
Input parameters processing module
↓
Mathematical modeling module
↓
Trajectory calculation and docking condition check
↓
Output results to console or file

## Development Environment Requirements

To run and develop the project, the following are required:

* Git
* CMake 3.20 or newer
* C++ compiler with C++17 support
* IDE or code editor:

* CLion, or
* Visual Studio / VS Code, or
* any other editor with C++ support

## Environment Setup

### Windows

1. Install Git
2. Install CMake
3. Install C++ compiler:

* MinGW, or
* Visual Studio Build Tools, or
* MSVC via Visual Studio

### Linux

1. Install Git
2. Install CMake
3. Install g++

For example, for Ubuntu:

```
sudo apt update
sudo apt install git cmake g++
```

## Клонування репозиторію

```
git clone https://github.com/OhChilled/Project.git
cd untitled2
```

## Збірка проєкту

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Запуск проєкту

Після успішної збірки виконуваний файл буде знаходитися в каталозі `build`.

### Linux

```
./untitled2
```

### Windows

```
.\untitled2.exe
```

## Запуск у режимі розробки

1. Внести зміни у вихідний код
2. Перезібрати проєкт:

```
cmake --build .
```

3. Повторно запустити виконуваний файл

## Робота з тестами

Якщо тести налаштовані в CMake, їх можна запустити так:

```
ctest
```

## Базові команди

### First Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

### Rebuild

```
cd build
cmake --build .
```

### Running Tests

```
cd build
ctest
```
