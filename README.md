##This is my computer graphic homework
It's really hard to made a great UI with SDL, especially with openGL.

##Feature I made
- Rotates along x, y, z axis
- Moves along x, y, z axis
- Scales along x, y, z axis
- OBJ file loader( but could only load one objec, it gets error when there's more than one object in OBJ )
- Shader load from external file
- Three views in the window( face to -y axis, -x axis and -z axis )
- hand made version of glOrtho()


##Some functions I Did NOT make
- setViewport, I use the API in openGL
- cliping, I don't know what should I do.


##What these file about
- main.cpp		Nothing to talk
- timer.cpp		To limit the maximum frame per second(fps)
- functions.cpp		Some functions I made for matrix operations

( not sure if I named it right ) 
- vertexShader		File for GLSL to process vertice
- fragmentShader	File for GLSL to process color and something else 

- basicNeed.h		Some header that used in most files
