# 3D_Graphics_renderer

### Dependencies ###
```` 
sudo apt install build-essential cmake libgl1-mesa-dev libsdl2-dev libglm-dev libsdl2-image-dev
````

#### Libraries ####
mkdir externals

##### stdb_image #####
Download stb_image.h from href{https://github.com/nothings/stb/blob/master/stb_image.h}, into externals/stb_image.h
make an externals/stb_image.cpp file that contains only the following
\"
 #define STB_IMAGE)IMPLEMENTAION 
 #include "stb_image.h"
\"
or see instructions within stb_image.h file (This is to get the cpp implementation)

##### glm (OpenGL Mathematics) #####

https://github.com/g-truc/glm
unzip within externals

##### Glad #####
retrived via webservies (some online glad loader)
set langauge to c/cpp
use glad version 3.3+
Spec = openGL
profile = Core NOT compatabiliy
for API,
    gl gives standard desktop functions
    gles1 gles2 (GL Embedded Systems) android/embedded dev etc. 
    glsc also appears to be embedded
can use mutiple for cross platform but code must conditionally call correct header API
add all extententions
tick "generate loader"
downlaod zip, put into externals/glad

