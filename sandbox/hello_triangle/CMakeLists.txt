cmake_minimum_required(VERSION 3.20)
project(hello_triangle_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(hello_triangle_demo
    main.cpp
    RenderWindow.cpp
)

# So you can do: #include <RenderWindow.h> (since it's in the project root)
target_include_directories(hello_triangle_demo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# SDL2 + SDL2_image via pkg-config (robust on Ubuntu)
find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)

target_link_libraries(hello_triangle_demo PRIVATE
    PkgConfig::SDL2
    PkgConfig::SDL2_IMAGE
)

# Optional: helpful warnings (safe defaults)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(hello_triangle_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
