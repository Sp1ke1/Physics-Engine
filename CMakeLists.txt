
cmake_minimum_required(VERSION 3.14)
project(PhysicsEngine LANGUAGES C CXX)
include( FetchContent ) # Libraries download
set(CMAKE_CXX_STANDARD 20)


# Raylib
FetchContent_Declare(
raylib
GIT_REPOSITORY https://github.com/raysan5/raylib.git
GIT_TAG 5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# Engine 
file(GLOB_RECURSE PHYSICS_ENGINE_SOURCE "${PROJECT_SOURCE_DIR}/PhysicsEngine/Source/*.cpp")
set ( PHYSICS_ENGINE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/PhysicsEngine/Include" )
add_library(PhysicsEngineLib STATIC ${PHYSICS_ENGINE_SOURCE})
target_link_libraries(PhysicsEngineLib PUBLIC raylib)
target_include_directories(PhysicsEngineLib PUBLIC ${PHYSICS_ENGINE_INCLUDE_DIR})

# GTest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()


# Tests 
add_executable(PhysicsEngineTest ${PROJECT_SOURCE_DIR}/Test_Main.cpp)
target_include_directories(PhysicsEngineTest PRIVATE ${PHYSICS_ENGINE_INCLUDE_DIR})
target_link_libraries(PhysicsEngineTest PRIVATE PhysicsEngineLib gtest_main)
add_test(NAME PhysicsEngineTest COMMAND PhysicsEngineTest)

# Example 
add_executable(PhysicsEngineExample ${PROJECT_SOURCE_DIR}/Main.cpp)
target_link_libraries(PhysicsEngineExample PRIVATE PhysicsEngineLib )



