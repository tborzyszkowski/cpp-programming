cmake_minimum_required(VERSION 3.14)
project(CalculatorTests CXX)

set(CMAKE_CXX_STANDARD 17)

# --- Google Test via FetchContent ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(calculator_tests
    src/calculator_tests.cpp
)

target_include_directories(calculator_tests PRIVATE src)

target_link_libraries(calculator_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(calculator_tests)
