/*

functions.h

author: TheKK <thumbd03803@gmail.com>
date: 10/16/2013

*/
#include "basicNeed.h"

using namespace std;

void setOrtho ( float projMat[4][4], float left, float right, float bottom, float top, float near, float far );

void rotationX ( float matrix[4][4], double degree );

void rotationY ( float matrix[4][4], double degree );

void rotationZ ( float matrix[4][4], double degree );

bool loadOBJ ( string filePath, vector<GLfloat> &vertex, vector<GLuint> &element, vector<GLfloat> &normal );

string loadShaderSource ( string filePath );

