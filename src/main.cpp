/*
main.cpp
*/
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SOIL/SOIL.h>
#include "timer.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = SCREEN_WIDTH;
const int SCREEN_BPP = 32;

const int FRAME_PER_SEC = 60;

SDL_Window *glWindow = NULL;

SDL_GLContext glContext;

GLuint vbo;		//Vertex Buffer Object
GLuint normalBuffer;	//Normal buffer
GLuint vao;		//Vertex Array Object
GLuint ebo;		//Element Buffer Object
GLuint vertexShader;	
GLuint fragmentShader;
GLuint shaderProgram;
GLuint posAttrib;	//Position Attribute

enum AXIS{ X_AXIS, Y_AXIS, Z_AXIS };
enum MODE{ ROTATE, TRANSLATE, SCALE };

int selectAxis = X_AXIS;
int selectMode = ROTATE;

GLuint uniRotaX;
float degreeX = 0;
float rotaXMat[][ 4 ] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniRotaY;
float degreeY = 0;
float rotaYMat[][ 4 ] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniRotaZ;
float degreeZ = 0;
float rotaZMat[][ 4 ] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniModel;
float modelMat[][ 4 ] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniView;
float viewMat[][ 4 ] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniProj;
float projMat[][ 4 ] = {	
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 1 }
};	

string loadShaderSource ( string filePath )
{	
	//Create file stream and check if there is an error
	ifstream source( filePath.c_str(), ios::in );
	if( !source.good() ){
		cout << "File open failed: " << filePath << endl;
		return "";
	}

	//Stream the file date into stringstream buffer	
	stringstream buffer;
	buffer << source.rdbuf();
	source.close();

	//Return the pointer of the string
	return buffer.str();
}	

bool loadOBJ ( string filePath, vector <GLfloat> *vertex, vector <GLuint> *element )
{	
	//Create a file stream and check if there is an error
	ifstream OBJsoruce( filePath.c_str(), ios::in );
	if( !OBJsoruce.good() ){
		cout << "OBJ file load failed: " << filePath << endl;
		return false;
	} 

	//Load line by line and search keyword to identify the data type( vertex or normal or ETC )
	string strTmp;
	while( getline( OBJsoruce, strTmp ) ){
		if( strTmp.find( "v" ) == 0 ){
			float x, y, z;	
			sscanf( strTmp.c_str(), "%*s %f %f %f", &x, &y, &z );

			vertex->push_back( x );
			vertex->push_back( y );
			vertex->push_back( z );
		}
		
		else if( strTmp.find( "f" ) == 0 ){
			int vertex1, vertex2, vertex3;
			int normal1, normal2, normal3;
			sscanf( strTmp.c_str(), "%*s %d//%d %d//%d %d//%d", &vertex1, &normal1, &vertex2, &normal2, &vertex3, &normal3 );

			element->push_back( vertex1 - 1 );			
			element->push_back( vertex2 - 1 );			
			element->push_back( vertex3 - 1 );			

			//normal.push_back( normal1 );
			//normal.push_back( normal2 );
			//normal.push_back( normal3 );
		}	
	}

	return true;
}
	
bool init ()
{	
	//Initiralize SDL subsystem
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )	return false;

	//Setup the window
	glWindow = SDL_CreateWindow( "OpenGL",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				SCREEN_WIDTH, SCREEN_HEIGHT,
				SDL_WINDOW_OPENGL );

	//Create openGL context	
	glContext = SDL_GL_CreateContext( glWindow );

	//Force GLEW to use morden OpenGL method for checking functions
	glewExperimental = GL_TRUE;
	glewInit();

	//Create a VertexArrayObject and copy the vertex date to it
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	//Generate a VertexBufferObject and store the vertec data in it
	vector <GLfloat> vertex;
	vector <GLfloat> nornaml;
	vector <GLuint> element;

	//Load data from OBJ file
	if( loadOBJ( "monkey.obj", &vertex, &element ) == false )	return false;	

	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER,
			vertex.size() * sizeof( GLfloat ),
			&vertex[ 0 ],
			GL_STATIC_DRAW
			);

	//Generate a ElementBufferObject and store the element date in it
	glGenBuffers( 1, &ebo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
			element.size() * sizeof( GLuint ),
			&element[ 0 ],
			GL_STATIC_DRAW
			);	


	/*SHADERS*/
	//Create and compile vertex shader
	string str = loadShaderSource( "shader/vertexShader" );
	if( str == "" )	return false;
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader,		//Shader type
			 1,			//Number of arrays to load
			(const GLchar**)&str,	//Path of the array
			NULL			//When loaded "NULL", terminate loading array
			);
	glCompileShader( vertexShader );

	//Create and compile fragment shader
	str = loadShaderSource( "shader/fragmentShader" );
	if( str == "" )	return false;
	fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader,
			1,
			(const GLchar**)&str,
			NULL
			);
	glCompileShader( fragmentShader );

	//Link the vertex and fragment shader into a shader program
	shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );	
	glBindFragDataLocation( shaderProgram, 0, "outColor");		///NEED MORE STUDY!!!
	glLinkProgram( shaderProgram );
	glUseProgram( shaderProgram );
	
	//Specity the layout of the vertex data			NEED MORE STUDY!!!
	posAttrib = glGetAttribLocation( shaderProgram, "position" );		
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0 );

	//Setup the clear color
	glClearColor( 0.0f, 0.0f, 0.2f, 1.0f );	

	//Enable depth cleaner
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );

	//Setup viewport and othometric
	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		
	//Check for error
	GLint status;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &status );
	if( status != GL_TRUE ){
		char vlog[ 255 ];
		glGetShaderInfoLog( vertexShader, 512, NULL, vlog );
		for( int i = 0; i < 255 ; i++ )		
			cout << vlog[ i ];
		cout << endl;
		return false;
	}
	
	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &status );
	if( status != GL_TRUE ){
		char flog[ 255 ];
		glGetShaderInfoLog( fragmentShader, 512, NULL, flog );
		for( int i = 0; i < 255; i++ )		
			cout << flog[ i ];
		cout << endl;
		return false;
	}

	GLenum error = glGetError();
	if( error != GL_NO_ERROR ){	
		cout << "error initialize OpenGL: " << gluErrorString( error ) << endl;
		return false;
	}

	return true;
}

void draw ()
{
	//Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//Draw a triangle from the three vertices
	glDrawElements( GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0 );		//Load 3 indices to draw, the data type is GLuint, and there is no offset

	//Swap window
	SDL_GL_SwapWindow( glWindow );			
}

void update ()
{
	//Update the matrixes
	glUniformMatrix4fv( uniRotaX, 1, GL_FALSE, (GLfloat*)rotaXMat );
	glUniformMatrix4fv( uniRotaY, 1, GL_FALSE, (GLfloat*)rotaYMat );
	glUniformMatrix4fv( uniRotaZ, 1, GL_FALSE, (GLfloat*)rotaZMat );

	glUniformMatrix4fv( uniModel, 1, GL_FALSE, (GLfloat*)modelMat );
	glUniformMatrix4fv( uniView, 1, GL_FALSE, (GLfloat*)viewMat );
	glUniformMatrix4fv( uniProj, 1, GL_FALSE, (GLfloat*)projMat );
}

void cleanUp ()	
{
	//Delete window
	SDL_DestroyWindow( glWindow );
	glWindow = NULL;

	//Delete context	
	SDL_GL_DeleteContext( glContext );

	//Delete shader and program
	glDeleteProgram( shaderProgram );
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	//Delete VBO and VAO	
	glDeleteBuffers( 1, &vbo );
	glDeleteVertexArrays( 1, &vao );

	//Exit SDL subsystem	
	SDL_Quit();
}

void eventHandler ( int key )
{
	switch( key ){

	//Change axis
	case SDLK_x:	selectAxis = X_AXIS;	break;
	case SDLK_y:	selectAxis = Y_AXIS;	break;
	case SDLK_z:	selectAxis = Z_AXIS;	break;

	//Change mode	
	case SDLK_s:	selectMode = SCALE;	break;
	case SDLK_r:	selectMode = ROTATE;	break;
	case SDLK_t:	selectMode = TRANSLATE;	break;

	//Reflection
	case SDLK_KP_1:	viewMat[0][0] *= -1;	break;
	case SDLK_KP_2:	viewMat[1][1] *= -1;	break;
	case SDLK_KP_3:	viewMat[2][2] *= -1;	break;

	//Increase or decrease the value	
	case SDLK_UP:
		switch( selectMode ){
		case TRANSLATE:
			switch( selectAxis ){
			case X_AXIS:	modelMat[3][0] += 0.05;	break;
			case Y_AXIS:	modelMat[3][1] += 0.05;	break;
			case Z_AXIS:	modelMat[3][2] += 0.05;	break;
			}
			break;
		case SCALE:
			switch( selectAxis ){
			case X_AXIS:	modelMat[0][0] += 0.05;	break;
			case Y_AXIS:	modelMat[1][1] += 0.05;	break;
			case Z_AXIS:	modelMat[2][2] += 0.05;	break;
			}
			break;
		case ROTATE:
			switch( selectAxis ){
			case X_AXIS:
				degreeX += 0.05;
				rotaXMat[1][1] = cos( degreeX );
				rotaXMat[2][1] =-1 * sin( degreeX );
				rotaXMat[1][2] = sin( degreeX );
				rotaXMat[2][2] = cos( degreeX );
				break;		
			case Y_AXIS:
				degreeY += 0.05;
				rotaYMat[0][0] = cos( degreeY );
				rotaYMat[0][2] =-1 * sin( degreeY );
				rotaYMat[2][0] = sin( degreeY );
				rotaYMat[2][2] = cos( degreeY );
				break;		
			case Z_AXIS:
				degreeZ += 0.05;
				rotaZMat[0][0] = cos( degreeZ );
				rotaZMat[1][0] =-1 * sin( degreeZ );
				rotaZMat[0][1] = sin( degreeZ );
				rotaZMat[1][1] = cos( degreeZ );
				break;		
			}
			break;
		}//End of selectMode switch
		break;

	case SDLK_DOWN:
		switch( selectMode ){
		case TRANSLATE:
			switch( selectAxis ){
			case X_AXIS:	modelMat[3][0] -= 0.05;	break;
			case Y_AXIS:	modelMat[3][1] -= 0.05;	break;
			case Z_AXIS:	modelMat[3][2] -= 0.05;	break;
			}
			break;
		case SCALE:
			switch( selectAxis ){
			case X_AXIS:	modelMat[0][0] -= 0.05;	break;
			case Y_AXIS:	modelMat[1][1] -= 0.05;	break;
			case Z_AXIS:	modelMat[2][2] -= 0.05;	break;
			}
			break;
		case ROTATE:
			switch( selectAxis ){
			case X_AXIS:
				degreeX -= 0.05;
				rotaXMat[1][1] = cos( degreeX );
				rotaXMat[2][1] =-1 * sin( degreeX );
				rotaXMat[1][2] = sin( degreeX );
				rotaXMat[2][2] = cos( degreeX );
				break;		
			case Y_AXIS:
				degreeY -= 0.05;
				rotaYMat[0][0] = cos( degreeY );
				rotaYMat[0][2] =-1 * sin( degreeY );
				rotaYMat[2][0] = sin( degreeY );
				rotaYMat[2][2] = cos( degreeY );
				break;		
			case Z_AXIS:
				degreeZ -= 0.05;
				rotaZMat[0][0] = cos( degreeZ );
				rotaZMat[1][0] =-1 * sin( degreeZ );
				rotaZMat[0][1] = sin( degreeZ );
				rotaZMat[1][1] = cos( degreeZ );
				break;		
			}
			break;
		}//End of selectMode switch
		break;
	}
}

int main ( int argc, char* argv[] )
{
	if( init() == false )	return 1;

	Timer fps;
	SDL_Event event;
	bool quit = false;

	//Get the uniform location to change their values
	uniRotaX = glGetUniformLocation( shaderProgram, "rotaX" );
	uniRotaY = glGetUniformLocation( shaderProgram, "rotaY" );
	uniRotaZ = glGetUniformLocation( shaderProgram, "rotaZ" );
	uniModel = glGetUniformLocation( shaderProgram, "model" );
	uniView = glGetUniformLocation( shaderProgram, "view" );
	uniProj = glGetUniformLocation( shaderProgram, "proj" );

	
	while( quit == false ){
		
		//Start to count for the limited frame rate	
		fps.start();

		//Event handler
		while( SDL_PollEvent( &event ) ){
			if( event.type == SDL_QUIT )	quit = true;
			if( event.type == SDL_KEYDOWN )	eventHandler( event.key.keysym.sym );
		}		
	
		update();

		draw();

		//Limit the max frame rate
		if( fps.getTicks() < 1000 / FRAME_PER_SEC )
			SDL_Delay( ( 1000 / FRAME_PER_SEC ) - fps.getTicks() );
	}	
	
	cleanUp();

	return 0;
}
