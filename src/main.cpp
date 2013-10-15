/*
main.cpp
*/
#include <iostream>
#include <sstream>
#include <fstream>
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

//Shader sources
const GLchar* vertexSource = 
		"#version 150 core\n"
		"in vec2 position;"
		"in vec3 color;"
		"in vec2 texcoord;"
		"out vec3 Color;"
		"out vec2 Texcoord;"
		"void main() {"
		"	Color = color;"
		"	Texcoord = texcoord;"
		"	position = position;"
		"	gl_Position = vec4( position, 0.0, 1.0 );"
		"}";

const GLchar* fragmentSource = 
		"#version 150 core\n"
		"in vec3 Color;"
		"in vec2 Texcoord;"
		"out vec4 outColor;"
		"uniform sampler2D texKitten;"
		"void main() {"
		"	outColor = texture( texKitten, Texcoord );"
		"}";

GLuint vbo;		//Vertex Buffer Object
GLuint vao;		//Vertex Array Object
GLuint ebo;		//Element Buffer Object
GLuint texture[ 2 ];		//Texture Buffer	
GLuint vertexShader;	
GLuint fragmentShader;
GLuint shaderProgram;
GLuint posAttrib;	//Position Attribute
GLuint colAttrib;	//Color Attribute
GLuint texAttrib;	//Texture Attribute
GLuint elements[] = {
	0, 1, 2,
	2, 3, 0	
};
GLfloat triangle[] = {
	-0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

string loadShaderSource( string filePath )
{

	ifstream source( filePath.c_str(), ios::in );
	if( !source.good() ){
		cout << "File open failed: " << filePath << endl;
		exit( 1 );
	}
	
	stringstream buffer;
	buffer << source.rdbuf();
	source.close();

	return buffer.str();
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
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( triangle ), triangle, GL_STATIC_DRAW );

	//Generate a ElementBufferObject and store the element date in it
	glGenBuffers( 1, &ebo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( elements ), elements, GL_STATIC_DRAW );	


	/*SHADERS*/
	//Create and compile vertex shader
	string str = loadShaderSource( "shader/vertexShader" );
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader,
			 1,
			(const GLchar**)&str,
			NULL
			);		// "1" mean there is only one char array for your source, "NULL" mean the array terminate at NULL character
	glCompileShader( vertexShader );

	//Create and compile fragment shader
	str = loadShaderSource( "shader/fragmentShader" );
	fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader,
			1,
			(const GLchar**)&str,
			NULL
			);			//same as the vertex shader
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
	glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0 );

	colAttrib = glGetAttribLocation( shaderProgram, "color" );
	glEnableVertexAttribArray( colAttrib );
	glVertexAttribPointer( colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)( 2 * sizeof(GLfloat) ) );

	//Setup the clear color
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	

	//Setup viewport
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
	glClear( GL_COLOR_BUFFER_BIT );
	
	//Draw a triangle from the three vertices
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );		//Load 3 indices to draw, the data type is GLuint, and there is no offset

	//Swap window
	SDL_GL_SwapWindow( glWindow );			
}

void update ()
{
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

	//Delete texture
	glDeleteTextures( 2, texture );
	
	//Exit SDL subsystem	
	SDL_Quit();
}
int main ( int argc, char* argv[] )
{
	if( init() == false )	return 1;

	Timer fps;
	SDL_Event event;
	bool quit = false;
	
	while( quit == false ){
		
		//Start to count for the limited frame rate	
		fps.start();

		//Event handler
		while( SDL_PollEvent( &event ) ){
			if( event.type == SDL_QUIT )	quit = true;
			if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q )	quit = true;
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
