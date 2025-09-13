
cmake_minimum_required(VERSION 3.14)
project(PhysicsEngine LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0
)


set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(PhysicsEngine main.cpp)

target_link_libraries(PhysicsEngine PRIVATE raylib)

