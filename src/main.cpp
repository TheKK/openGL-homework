/*

main.cpp
-Just a main.cpp

author: TheKK <thumbd03803@gmail.com>

*/
#include "basicNeed.h"
#include "functions.h"
#include "timer.h"

//Path of GLSL files
#define VERTEX_SHADER_PATH	"shader/basicShader.vs"
#define FRAGMENT_SHADER_PATH	"shader/basicShader.fs"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = SCREEN_WIDTH;

const int FRAME_PER_SEC = 60;

SDL_Window *glWindow = NULL;
SDL_GLContext glContext;
int glWindowID;

SDL_Window *subWindow = NULL;

bool windowed = true;
GLuint count = 0;

GLuint vbo;		//Vertex Buffer Object
GLuint elementNum;	//Number of loaded vertics

GLuint normalBuffer;	//Normal buffer
GLuint vao;		//Vertex Array Object
GLuint ebo;		//Element Buffer Object
GLuint ebo2;
GLuint vertexShader;	
GLuint fragmentShader;
GLuint shaderProgram;
GLint posAttrib;	//Position attribute
GLint normalAttrib;	//Normal attribute

enum AXIS{ X_AXIS, Y_AXIS, Z_AXIS };
enum MODE{ ROTATE, ROTATE_AXIS, TRANSLATE, SCALE, SHEAR };

int selectAxis = X_AXIS;
int selectMode = ROTATE;

GLuint uniRota;
float rotaMat[][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniRevoX;
float degreeX = 0;
float revoXMat[][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniRevoY;
float degreeY = 0;
float revoYMat[][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniRevoZ;
float degreeZ = 0;
float revoZMat[][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	
		
GLuint uniModel;
float shearX = 1.5;
float shearY = 1.5;
float shearZ = 1.5;
float modelMat[][ 4 ] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniView;
float viewMat[][ 4 ] = {
		{ 1, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0,-1, 0 ,0 },
		{ 0, 0, 0, 1 }
};	

GLuint uniProj;
float projMat[][ 4 ] = {	
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },	//Keep the z-axis value for depth test
		{ 0, 0, 0, 1 }
};	

GLuint uniViewport;
float viewportMat[][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
};

GLuint uniLight;
float lightPosition[] = { 0, -30, 0 };

bool
init ()
{	
	//Initiralize SDL subsystem
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )	return false;

	//Setup the window
	glWindow = SDL_CreateWindow(
			"OpenGL",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
			);
	glWindowID = SDL_GetWindowID( glWindow );
	if( glWindow == NULL )	return false;

	//Create openGL context	
	glContext = SDL_GL_CreateContext( glWindow );

	//Force GLEW to use morden OpenGL method for checking functions
	glewExperimental = GL_TRUE;
	glewInit();

	//Create a VertexArrayObject
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	//Generate a VertexBufferObject and store the vertex data into it
	vector<GLfloat> vertex;
	vector<GLfloat> normal;
	vector<GLuint> element;

	//Load data from OBJ file
	if( loadOBJ( "cube.obj", vertex, element, normal ) == false )	return false;	

	//Generate a VertexBufferObject and store the vertice data into it
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER,
			vertex.size() * sizeof(GLfloat),
			&vertex[ 0 ],
			GL_STATIC_DRAW
			);

	//Generate a ElementBufferObject and store the element data into it
	glGenBuffers( 1, &ebo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
			element.size() * sizeof(GLuint),
			&element[ 0 ],
			GL_STATIC_DRAW
			);	
	elementNum = element.size();
	
	//Generate a NormalBufferObject and store the normal data into it
	glGenBuffers( 1, &normalBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
	glBufferData( GL_ARRAY_BUFFER,
			normal.size() * sizeof(GLfloat),
			&normal[ 0 ],
			GL_STATIC_DRAW
			);

	/*SHADERS*/
	//Create and compile vertex shader
	string str = loadShaderSource( VERTEX_SHADER_PATH );
	if( str == "" )	return false;
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader,		//Shader type
			 1,			//Number of arrays to load
			(const GLchar**)&str,	//Path of the array
			NULL			//When loaded "NULL", terminate loading array
			);
	glCompileShader( vertexShader );

	//Create and compile fragment shader
	str = loadShaderSource( FRAGMENT_SHADER_PATH );
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
	/*get attrib means get the location of input entry of shader in program, data type is Uint*/
	/*And we use glVertexAttribPointer to connect buffer array and the location of input entry we just got*/
	posAttrib = glGetAttribLocation( shaderProgram, "position_modelspace" );		
	glEnableVertexAttribArray( posAttrib );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0 );

	normalAttrib = glGetAttribLocation( shaderProgram, "normal_modelspace" );		
	glEnableVertexAttribArray( normalAttrib );
	glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
	glVertexAttribPointer( normalAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0 );

	//Setup the clear color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	//Setup ortho	
	setOrtho( projMat,-2, 2, -2, 2, -5, 5 );

	//Enable depth cleaner
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );

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

	//Get uniform locations in GLSL 
	uniRevoX = glGetUniformLocation( shaderProgram, "revoX" );
	uniRevoY = glGetUniformLocation( shaderProgram, "revoY" );
	uniRevoZ = glGetUniformLocation( shaderProgram, "revoZ" );
	uniRota = glGetUniformLocation( shaderProgram, "rota" );
	uniModel = glGetUniformLocation( shaderProgram, "model" );
	uniView = glGetUniformLocation( shaderProgram, "view" );
	uniProj = glGetUniformLocation( shaderProgram, "proj" );
	uniViewport = glGetUniformLocation( shaderProgram, "viewport" );
	uniLight = glGetUniformLocation( shaderProgram, "lightPosition_space" );

	return true;
}

void
windowResize( int width, int height )
{
	glViewport( 0, 0, width, height );
}

void
draw ()
{
	//Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	if( count != elementNum ) 
		glDrawRangeElements( GL_TRIANGLES, 0, 0, 6*count, GL_UNSIGNED_INT, 0 );
	else
		glDrawRangeElements( GL_TRIANGLES, 0, elementNum, elementNum, GL_UNSIGNED_INT, 0 );
	
	//Swap window
	SDL_GL_SwapWindow( glWindow );			
}

void
update ()
{
	//Update the matrixes
	glUniformMatrix4fv( uniRevoX, 1, GL_TRUE, (GLfloat*)revoXMat );
	glUniformMatrix4fv( uniRevoY, 1, GL_TRUE, (GLfloat*)revoYMat );
	glUniformMatrix4fv( uniRevoZ, 1, GL_TRUE, (GLfloat*)revoZMat );
	glUniformMatrix4fv( uniRota, 1, GL_TRUE, (GLfloat*)rotaMat );

	//GL_TRUE means to transpose the matrix before applying, because matrix in GLSL is column major
	//We set as GL_TRUE here
	glUniformMatrix4fv( uniModel, 1, GL_TRUE, (GLfloat*)modelMat );
	glUniformMatrix4fv( uniView, 1, GL_TRUE, (GLfloat*)viewMat );
	glUniformMatrix4fv( uniProj, 1, GL_TRUE, (GLfloat*)projMat );
	glUniformMatrix4fv( uniViewport, 1, GL_TRUE, (GLfloat*)viewportMat );

	//Light position
	glUniform3fv( uniLight, 1, (GLfloat*)lightPosition ); 

	//This counter is used for special effect
	if( count < elementNum/3 )	count++;
}

void
cleanUp ()	
{
	//Delete window
	SDL_DestroyWindow( glWindow );
	glWindow = NULL;

	//Delete openGL context	
	SDL_GL_DeleteContext( glContext );

	//Delete shaders and program
	glDeleteProgram( shaderProgram );
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	//Delete VBO, NBO and VAO	
	glDeleteBuffers( 1, &vbo );
	glDeleteBuffers( 1, &normalBuffer );
	glDeleteVertexArrays( 1, &vao );

	//Exit SDL subsystem	
	SDL_Quit();
}

void
eventHandler ( int key )
{
	switch( key ){

	//Change target axis
	case SDLK_x:	selectAxis = X_AXIS;	break;
	case SDLK_y:	selectAxis = Y_AXIS;	break;
	case SDLK_z:	selectAxis = Z_AXIS;	break;
	
	//Special effect
	case SDLK_e:
		if( count == elementNum/3 )	
			count = 0;
		else
			count = elementNum/3;
		break;

	//Change mode	
	case SDLK_s:	selectMode = SCALE;		break;
	case SDLK_r:	selectMode = ROTATE;		break;
	case SDLK_v:	selectMode = ROTATE_AXIS;	break;
	case SDLK_t:	selectMode = TRANSLATE;		break;
	case SDLK_h:	selectMode = SHEAR;		break;

	//Reflection
	case SDLK_KP_1:	modelMat[0][0] *= -1;	break;
	case SDLK_KP_2:	modelMat[1][1] *= -1;	break;
	case SDLK_KP_3:	modelMat[2][2] *= -1;	break;

	//Toggle fullscreen
	case SDLK_RETURN:	
		if( windowed == true ){
			SDL_SetWindowFullscreen( glWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
			windowed = false;
		}
		else{
			SDL_SetWindowFullscreen( glWindow, 0 );
			windowed = true;
		}	
		break;	

	//Change the projection mode
	case SDLK_KP_5:	
		if( modelMat[3][2] == 0 ){
			modelMat[3][2] = 100;	
			setPerspec( projMat,-5, 5, 5,-5, 1, 10 );
		}
		else{
			modelMat[3][2] = 0;
			setOrtho( projMat,-2, 2, -2, 2, -5, 5 );
		}
		break;

	//Increase or decrease the value	
	case SDLK_UP:
		switch( selectMode ){
		case TRANSLATE:
			switch( selectAxis ){
			case X_AXIS:	modelMat[0][3] += 0.05;	break;
			case Y_AXIS:	modelMat[1][3] += 0.05;	break;
			case Z_AXIS:	modelMat[2][3] += 0.05;	break;
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
				rotationX( rotaMat, 0.1 );	break;		
			case Y_AXIS:
				rotationY( rotaMat, 0.1 );	break;		
			case Z_AXIS:
				rotationZ( rotaMat, 0.1 );	break;		
			}
			break;
		case ROTATE_AXIS:
			switch( selectAxis ){
			case X_AXIS:
				degreeX += 0.05;
				revoXMat[1][1] = cos( degreeX );
				revoXMat[2][1] = sin( degreeX );
				revoXMat[1][2] = -1 * sin( degreeX );
				revoXMat[2][2] = cos( degreeX );
				break;
			case Y_AXIS:
				degreeY += 0.05;
				revoYMat[0][0] = cos( degreeY );
				revoYMat[2][0] = -1 * sin( degreeY );
				revoYMat[0][2] = sin( degreeY );
				revoYMat[2][2] = cos( degreeY );
				break;
			case Z_AXIS:
				degreeZ += 0.05;
				revoZMat[0][0] = cos( degreeZ );
				revoZMat[1][0] = sin( degreeZ );
				revoZMat[0][1] = -1 * sin( degreeZ );
				revoZMat[1][1] = cos( degreeZ );
				break;
			}
			break;	
		case SHEAR:
			switch( selectAxis ){
			case X_AXIS:
				shearX += 0.05;
				modelMat[0][1] = 1 / tan( shearX );
			case Y_AXIS:
				shearY += 0.05;
				modelMat[1][0] = 1 / tan( shearY );
			case Z_AXIS:
				shearZ += 0.05;
				modelMat[2][1] = 1 / tan( shearZ );
			}
			break;
		}//End of selectMode switch
		break;

	case SDLK_DOWN:
		switch( selectMode ){
		case TRANSLATE:
			switch( selectAxis ){
			case X_AXIS:	modelMat[0][3] -= 0.05;	break;
			case Y_AXIS:	modelMat[1][3] -= 0.05;	break;
			case Z_AXIS:	modelMat[2][3] -= 0.05;	break;
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
				rotationX( rotaMat,-0.1 );	break;		
			case Y_AXIS:
				rotationY( rotaMat,-0.1 );	break;		
			case Z_AXIS:
				rotationZ( rotaMat,-0.1 );	break;		
			}
			break;
		case ROTATE_AXIS:
			switch( selectAxis ){
			case X_AXIS:
				degreeX -= 0.05;
				revoXMat[1][1] = cos( degreeX );
				revoXMat[2][1] = sin( degreeX );
				revoXMat[1][2] = -1 * sin( degreeX );
				revoXMat[2][2] = cos( degreeX );
				break;
			case Y_AXIS:
				degreeY -= 0.05;
				revoYMat[0][0] = cos( degreeY );
				revoYMat[2][0] = -1 * sin( degreeY );
				revoYMat[0][2] = sin( degreeY );
				revoYMat[2][2] = cos( degreeY );
				break;
			case Z_AXIS:
				degreeZ -= 0.05;
				revoZMat[0][0] = cos( degreeZ );
				revoZMat[1][0] = sin( degreeZ );
				revoZMat[0][1] = -1 * sin( degreeZ );
				revoZMat[1][1] = cos( degreeZ );
				break;
			}
			break;
		}//End of selectMode switch
		break;
	}
}

/*void
effect ()
{
	//Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	SDL_GL_SwapWindow( glWindow );			

	//Draw a triangle from the three vertices
}*/

int
main ( int argc, char* argv[] )
{
	if( init() == false ){
		cleanUp();
		return 1;
	}	

	Timer fps;
	SDL_Event event;
	bool quit = false;
	count = elementNum/3;

	string str("vn 1 1 1");
	size_t pos = 0;
	
	pos = str.find( "v", 0, 1 );
	cout << pos << endl;

	while( quit == false ){
		
		//Start to count for the limited frame rate	
		fps.start();

		//Event handler
		while( SDL_PollEvent( &event ) ){
			if( event.type == SDL_QUIT )	quit = true;
			else if( event.type == SDL_KEYDOWN )
				eventHandler( event.key.keysym.sym );

			else if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
				windowResize( event.window.data1, event.window.data2 );

			else if( event.type == SDL_MOUSEWHEEL ){
				if( event.wheel.y > 0 )	lightPosition[1] += 1;
				else			lightPosition[1] -= 1;
			}
			
			else if( event.type == SDL_MOUSEMOTION ){
				lightPosition[0] =-25 + 50*( (float)event.motion.x / SCREEN_WIDTH );
				lightPosition[2] = 25 - 50*( (float)event.motion.y / SCREEN_HEIGHT );
			}
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
