cmake_minimum_required(VERSION 4.2)
project(Rasterizer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.18
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

add_executable(Rasterizer src/main.cpp src/window.cpp)

target_link_libraries(Rasterizer PRIVATE SDL3::SDL3-static)