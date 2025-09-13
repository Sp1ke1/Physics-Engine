
cmake_minimum_required(VERSION 3.14)
project(PhysicsEngine LANGUAGES C CXX)
include( FetchContent ) # Libraries download


# Raylib
FetchContent_Declare(
raylib
GIT_REPOSITORY https://github.com/raysan5/raylib.git
GIT_TAG 5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# Testing
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

set (PHYSICS_ENGINE_TEST_SOURCE ${PROJECT_SOURCE_DIR}/Source/Test/Test_Main.cpp)
add_executable(PhysicsEngineTests ${PHYSICS_ENGINE_TEST_SOURCE})
target_include_directories(PhysicsEngineTests PRIVATE ${PROJECT_SOURCE_DIR}/Source)
target_link_libraries(PhysicsEngineTests PRIVATE gtest_main )

add_test(NAME PhysicsEngineTests COMMAND PhysicsEngineTests)


# Physics engine executablee
set (PHYSICS_ENGINE_SOURCE ${PROJECT_SOURCE_DIR}/Source/PhysicsEngine/Main.cpp)
add_executable(PhysicsEngine ${PHYSICS_ENGINE_SOURCE})
target_include_directories(PhysicsEngine PRIVATE ${PROJECT_SOURCE_DIR}/Source)
target_link_libraries(PhysicsEngine PRIVATE raylib)



