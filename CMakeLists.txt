cmake_minimum_required(VERSION 3.20)
project(untitled2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Main app ----
add_executable(untitled2
        cosmos2025.cpp
        library.cpp
)
target_include_directories(untitled2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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
