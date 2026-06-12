#CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
    project(models LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #so that IDE can see my -I flags

#PRIVATE: only this target uses it, 
#PUBLIC: this target uses it and anything that links to this target
#INTERFACE: this target doesn't compile srcs but consumers get it (header only libs often)

#GLAD
add_library(glad STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/externals/glad/src/glad.c"
)
#target_include_directories = where headers live
#target_link_directories = where LIBRARIES live
target_include_directories(glad PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/externals/glad/include"
)
# add executable =  src code, i.e. cpp files I want compiled to .objs, that are then linked| .h files dont need to be compiled but can be just for IDE sanity. 
# Makes first arg a target, so must come before target_include_directories(target_name...
add_executable(models 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/externals/stb_image.cpp"
)
#sets header search paths e.g. when searching compiling "target" search these dirs for headers
target_include_directories( models PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/externals"
    "${CMAKE_CURRENT_SOURCE_DIR}/externals/assimp/include"
)


# SDL2 + SDL2_image via pkg-config (robust on Ubuntu)
find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)


find_package(OpenGL REQUIRED)

target_link_libraries(models  PRIVATE 
    glad 
    OpenGL::GL
    dl 
    PkgConfig::SDL2
    PkgConfig::SDL2_IMAGE
    "${CMAKE_CURRENT_SOURCE_DIR}/externals/assimp/build/lib/libassimp.a"
)


# Optional: helpful warnings (safe defaults)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(models PRIVATE -Wall -Wextra -Wpedantic)
endif()
