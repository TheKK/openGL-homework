
#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <SDL/SDL_opengl.h>
#include <cmath>

using namespace std;

void rotateX( GLdouble vertex[][3], double degree );
void rotateY( GLdouble vertex[][3], double degree );
void rotateZ( GLdouble vertex[][3], double degree );

#endif
