
#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <SDL/SDL_opengl.h>
#include <cmath>

using namespace std;

void rotateX( GLdouble vertex[][ 3 ], double degree );
void rotateY( GLdouble vertex[][ 3 ], double degree );
void rotateZ( GLdouble vertex[][ 3 ], double degree );

void tranlate( GLdouble vertex[][ 3 ], GLdouble x, GLdouble y, GLdouble z );

void scale( GLdouble vertex[][ 3 ], GLdouble x, GLdouble y, GLdouble z );

#endif
