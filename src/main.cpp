/*
main.cpp
*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include "timer.h"
#include "func.h"

using namespace std;

enum axis { X_AXIS, Y_AXIS, Z_AXIS };
enum mode { TRANSLATION, ROTATION, SCALE };

const short int SCREEN_HEIGHT = 500;
const short int SCREEN_WIDTH = SCREEN_HEIGHT;
const GLdouble SCREEN_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;
const short int SCREEN_BPP = 32;

const short int FRAMES_PER_SECOND = 60;

SDL_Window *glScreen = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Surface *surface = NULL;
SDL_GLContext glContext;

SDL_Event event;

bool quit = false;
bool renderQuad = true;
bool windowed = true;

int axis = X_AXIS;
int mode = TRANSLATION;

GLdouble cube[][ 3 ] = { { 10, 10, 10 }, {-10, 10, 10 }, {-10,-10, 10}, { 10,-10, 10 },
		{ 10, 10,-10 }, {-10, 10,-10 }, {-10,-10,-10}, { 10,-10,-10} };

int lines[][ 2 ] ={ { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 1 },
		{ 1, 5 }, { 2, 6 }, { 3, 7 }, { 4, 8 },
		{ 5, 6 }, { 6, 7 }, { 7, 8 }, { 8, 5 } };

int face[][ 4 ] = { { 1, 2 , 3, 4 }, { 5, 6, 7, 8 }, { 2, 3, 7, 6 }, { 1, 4, 8, 5 }, { 3, 4, 8, 7 }, { 1, 2, 6, 5 } };

bool initGL()
{	
	//Set as double buffer
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);


	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	//Set the projection property
	glOrtho( -20, 20, 20, -20, 50, -50 );


	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );

	//Check for error
	GLenum error = glGetError();
	if( error != GL_NO_ERROR ){
		cout << "Error initializing OpenGL! " << gluErrorString( error ) << endl;
		return false;
	}


	return true;
}	

bool init()
{
	//initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )	return false;

	

	//Create Window
	glScreen = SDL_CreateWindow( "OpenGL test",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

	renderer = SDL_CreateRenderer( glScreen, -1, SDL_RENDERER_ACCELERATED );
	SDL_SetRenderDrawColor( renderer, 0, 255, 0, 0 );
	SDL_RenderSetLogicalSize( renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Initialize OpenGL
	if( initGL() == false )	return false;

	//Load img
		
	return true;
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	switch( key ){
	case 'q':	renderQuad = !renderQuad;	break;
	case SDLK_ESCAPE:	quit = true;	break;
	case 'x':	axis = 0;	break;
	case 'y':	axis = 1;	break;
	case 'z':	axis = 2;	break;
	case '1':	mode = TRANSLATION;	break;
	case '2':	mode = ROTATION;	break;
	case '3':	mode = SCALE;		break;
	case SDLK_RETURN:
		if( windowed ){
			windowed = !windowed;
			SDL_SetWindowFullscreen( glScreen, SDL_WINDOW_FULLSCREEN_DESKTOP );
		}
		else{	
			windowed = !windowed;
			SDL_SetWindowFullscreen( glScreen, 0 );
		}
		break;
	}	
}

void resize( int width, int height )
{
	GLdouble resizeRaiot = GLdouble( width ) / GLdouble( height );
	GLint blank;
	GLint portWidth;
	GLint portHeight;
	
	if( resizeRaiot > SCREEN_RATIO ){
		portHeight = GLint( height );
		portWidth = portHeight * SCREEN_RATIO;
		blank = ( GLint( width ) - portWidth ) / 2;

		glViewport( blank, 0, portWidth, portHeight );
	}
	else if( resizeRaiot < SCREEN_RATIO ){
		portWidth = GLint( width );
		portHeight = portWidth / SCREEN_RATIO;
		blank = ( GLint( height ) - portHeight ) / 2;

		glViewport( 0, blank, portWidth, portHeight );
	}
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	

	glOrtho( -1 * portWidth / 2, portWidth / 2, portHeight / 2, -1 * portHeight / 2, 20, -20 );
}

void update()
{

}

void render()
{	
	glClear( GL_COLOR_BUFFER_BIT );

	glColor3ub( 255, 255, 255 );
	glRecti( -20, -20, 20, 20 );
	//Render quad
	if( renderQuad == true ){
		glColor3f( 0, 100, 255 );
		glBegin( GL_QUADS );
			for( int i = 0; i < 1; i++ )
				for( int j = 0; j < 4; j++ )
					glVertex3dv( cube[ face[ i ][ j ] ]);
		glEnd();
	}	
}

void cleanUp()
{	
	SDL_DestroyWindow( glScreen );
	glScreen = NULL;

	SDL_DestroyTexture( texture );
	texture = NULL;

	SDL_Quit();
}

int main( int argc, char* argv[] )
{
	Timer fps;
	double scaleSize = 1;
	if( init() == false ){
		cout << "error" << endl;
		return 1;
	}
	
	surface = SDL_LoadBMP( "menuLogo.bmp" );
	texture = SDL_CreateTextureFromSurface( renderer, surface );
	SDL_FreeSurface( surface );

	while( quit == false ){
		
		fps.start();

		while( SDL_PollEvent( &event ) ){
				
			//Window resize event
			if( event.type == SDL_QUIT )	quit = true;
			if( event.type == SDL_KEYDOWN ){
				int x = 0, y = 0;
				SDL_GetMouseState( &x, &y );
				handleKeys( event.key.keysym.sym, x, y );
			}
			if( event.type == SDL_MOUSEWHEEL ){
				switch( mode ){
				case TRANSLATION:
					switch( axis ){
					case X_AXIS:	tranlate( cube, event.wheel.y, 0, 0 );	break;
					case Y_AXIS:	tranlate( cube, 0, event.wheel.y, 0 );	break;
					case Z_AXIS:	tranlate( cube, 0, 0, event.wheel.y );	break;
					}
					break;
				case ROTATION:
					switch( axis ){
					case X_AXIS:	rotateX( cube, event.wheel.y / 10.0 );	break;
					case Y_AXIS:	rotateY( cube, event.wheel.y / 10.0 );	break;
					case Z_AXIS:	rotateZ( cube, event.wheel.y / 10.0 );	break;
					}
					break;
				case SCALE:
					switch( axis ){
					case X_AXIS:	scale( cube, scaleSize, 1, 1 );	break;
					case Y_AXIS:	scale( cube, 1, scaleSize, 1 );	break;
					case Z_AXIS:	scale( cube, 1, 1, scaleSize );	break;
					}
					break;
				}
			}
			if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
				resize( event.window.data1, event.window.data2 );
		}
	
		update();
		
		render();

		SDL_GL_SwapWindow( glScreen );

		if( fps.getTicks() < 1000 / FRAMES_PER_SECOND )
			SDL_Delay( 1000 / FRAMES_PER_SECOND - fps.getTicks() );

	}
	cleanUp();
		
	return 0;
}
