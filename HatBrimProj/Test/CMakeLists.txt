# ---------------------------------------------
# HOST ONLY: builds the unit tests on the PC (Windows or Linux/WSL). Not part of the STM32 build.
cmake_minimum_required(VERSION 3.14)
project(HatTests CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# WHY: FetchContent downloads GoogleTest at configure time, so you install nothing by hand.
include(FetchContent)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# WHY CHANGED: was a .zip URL download. fetching through git is more robust (works wherever
# git can reach GitHub, which you already need for your project) and avoids the
# CMP0135 timestamp warning.
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# WHY: on Windows/MSVC gtest must use the same C runtime as our test, otherwise linking fails.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# WHY: only the real production .cpp files under test are compiled. nothing from CubeIDE.
add_executable(light_sensor_tests
  LightSensorTest.cpp
  ../App/Hardware/LightSensor.cpp
  ../App/Hardware/AdcScanner.cpp
)

# WHY: order matters. 'fake' comes first so "main.h" resolves to our fake, not the STM32 one.
# '../App' lets the production code's own includes like "Hardware/AdcScanner.h" work unchanged.
target_include_directories(light_sensor_tests PRIVATE fake ../App)

target_link_libraries(light_sensor_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(light_sensor_tests)
# ---------------------------------------------
