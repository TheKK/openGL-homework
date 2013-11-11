##Ok, it's time for the next challenge, SHADING!
I'm more familar with SDL2 so this time might be ... more hard.

##Feature I made
- Rotates along x, y, z axis
- Moves along x, y, z axis
- Scales along x, y, z axis
- Shear along x, y, z axis
- OBJ file loader( but could only load one objec, it gets error when there's more than one object in OBJ )
- Shader load from external file
- Three views in the window( face to -y axis, -x axis and -z axis )
- hand made version of glOrtho()

##Some functions I Did NOT make
- setViewport, I use the API in openGL
- cliping, I don't know what should I do.

##What these files about
- src/main.cpp			Nothing to talk, it's main.cpp
- src/timer.cpp			A timer class to limit the maximum frame per second(fps)
- src/functions.cpp		Some functions I made for matrix operations
- shader/vertexShader		File for GLSL to process vertice
- shader/fragmentShader		File for GLSL to process color and something else 
- basicNeed.h			Some header that used in most files

##Development environment
OS: ubuntu 13.04 x86_64
Library: SDL2, openGL, glew

