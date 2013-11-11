/*

object.h
-Class for OBJ object

author: TheKK <thumbd03803@gmail.com>

*/

#ifndef OBJECT_H
#define OBJECT_H

#include "basicNeed.h"

using namespace std;

class Object
{
	public:
		//Constructor
		Object( char* OBJFilePath );

		//OBJ loader
		bool loadOBJ();

		void free();
		
	private:
		//Name of object	
		char* mObjectName;

		//Path of object
		char* mFilePath;	

		//Object data
		GLuint mEbo;
		GLuint mVertex;	
		GLuint mNormal;
		GLuint mTexture;
		short int mElementNumber;

};

#endif
