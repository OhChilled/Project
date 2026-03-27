cmake_minimum_required(VERSION 3.20)
project(untitled2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(spdlog-1.x)

# ---- Main app ----
add_executable(untitled2
        cosmos2025.cpp
        library.cpp
        logger.cpp
)
target_include_directories(untitled2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(untitled2 PRIVATE spdlog::spdlog)

# ---- Tests (GoogleTest) ----
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
        tests/test_model.cpp
        library.cpp
)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()

add_custom_target(lint
    COMMAND clang-tidy
        ${CMAKE_SOURCE_DIR}/cosmos2025.cpp
        ${CMAKE_SOURCE_DIR}/library.cpp
        ${CMAKE_SOURCE_DIR}/logger.cpp
        --
        -I${CMAKE_SOURCE_DIR}
        -I${CMAKE_SOURCE_DIR}/spdlog-1.x/include
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()