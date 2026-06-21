# 3D_Graphics_renderer

Note repo currently contains hardcoded paths that will need to be updated if ported

### Dependencies

```
sudo apt install build-essential cmake libgl1-mesa-dev libsdl2-dev libglm-dev libsdl2-image-dev
```

#### Libraries

mkdir externals

##### stdb_image

Download stb_image.h from href{https://github.com/nothings/stb/blob/master/stb_image.h}, into externals/stb_image.h
make an externals/stb_image.cpp file that contains only the following\
\"\
 #define STB_IMAGE)IMPLEMENTAION \
 #include "stb_image.h"\
\"\

or see instructions within stb_image.h file (This is to get the cpp implementation)

##### glm (OpenGL Mathematics)

https://github.com/g-truc/glm \
unzip within externals \

##### Glad

retrived via webservies (some online glad loader) \
set langauge to c/cpp \
use glad version 3.3+ \
Spec = openGL \
profile = Core NOT compatabiliy \
for API, \
\t gl gives standard desktop functions \
\t gles1 gles2 (GL Embedded Systems) android/embedded dev etc. \
\t glsc also appears to be embedded \
can use mutiple for cross platform but code must conditionally call correct header API \
add all extententions \
tick "generate loader" \
downlaod zip, put into externals/glad \

##### Assimp

cd externals \
git clone https://github.com/assimp/assimp.git \
cd assimp \
cmake CMakeLists.txt -DASSIMP_BUILD_ASSIMP_TOOLS=ON -DBUILD_SHARED_LIBS=OFF \
cmake --build . \
defualt builds a .so shared library, this means smaller executable but runtime depends on .so code and can run into loader errors.
For this project we'll use static for ease of use. This comes at the trade off of larger executable, longer link-times, required to rebuild for newer Assimp. 
Hence the DBUILD_SHARED_LIB=OFF flag in the cmake command above. \ 

There should now be a "./build/lib/libassimp.a" and a "./generated/assimpConfig.cmake" file if everything went smoothly

##### Json Parser

should be simple enough. Downlaod json.hpp from href{https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp} \ 
move it to externals/ done.
