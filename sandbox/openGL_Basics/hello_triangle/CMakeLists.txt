cmake_minimum_required(VERSION 3.20)
project(hello_triangle_demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#GLAD
add_library(glad STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/externals/src/glad.c"
)
target_include_directories(glad PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/externals/include"
)

add_executable(hello_triangle_demo
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RenderWindow.cpp"
)




# So you can do: #include <RenderWindow.h> (since it's in the project root)
target_include_directories(hello_triangle_demo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# SDL2 + SDL2_image via pkg-config (robust on Ubuntu)
find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)




find_package(OpenGL REQUIRED)

target_link_libraries(hello_triangle_demo PRIVATE 
    glad 
    OpenGL::GL
    dl 
    PkgConfig::SDL2
    PkgConfig::SDL2_IMAGE
)


# Optional: helpful warnings (safe defaults)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(hello_triangle_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
