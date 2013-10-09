/*
main.cpp
*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "timer.h"

using namespace std;

const short int SCREEN_HEIGHT = 500;
const short int SCREEN_WIDTH = SCREEN_HEIGHT ;
const GLdouble SCREEN_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;
const short int SCREEN_BPP = 32;

const short int FRAMES_PER_SECOND = 60;

SDL_Window *glScreen = NULL;
SDL_Renderer *renderer = NULL;

SDL_Event event;

bool quit = false;
bool renderQuad = true;
bool windowed = true;

bool initGL()
{	
	//Set as double buffer
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Initialize clear color
	glClearColor( 1.f, 0.f, 0.f, 1.f );
/*
	//Check for error
	GLenum error = glGetError();
	if( error != GL_NO_ERROR ){
		cout << "Error initializing OpenGL! " << gluErrorString( error ) << endl;
		return false;
	}
*/
	return true;
}	

bool init()
{
	//initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )	return false;

	//Create Window
	glScreen = SDL_CreateWindow( "OpenGL test",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL );

	renderer = SDL_CreateRenderer( glScreen, -1, 0 );

	SDL_RenderSetLogicalSize( renderer, SCREEN_WIDTH, SCREEN_HEIGHT );

	if( glScreen == NULL )	return false;
	
	//Initialize OpenGL
	if( initGL() == false )	return false;

	return true;
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	switch( key ){
	case 'q':	renderQuad = !renderQuad;	break;
	case 'w':	quit = true;	break;
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

		glViewport( blank, 0, portWidth, portHeight );
	}
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	
}

void update()
{

}

void render()
{	
	//Clear color buffer
	glClear( GL_COLOR_BUFFER_BIT );
	
	//Render quad
	if( renderQuad == true ){
		glColor3f( 0, 255, 255 );
		glBegin( GL_QUADS );
			glVertex2f( -0.5f, -0.5f );
			glVertex2f(  0.5f, -0.5f );
			glVertex2f(  0.5f,  0.5f );
			glVertex2f( -0.5f,  0.5f );
		glEnd();
	}

	//Update screen
	SDL_GL_SwapWindow( glScreen );
}

void cleanUp()
{
	SDL_DestroyWindow( glScreen );
	SDL_DestroyRenderer( renderer );
}

int main( int argc, char* argv[] )
{
	Timer fps;
	if( init() == false ){
		cout << "error" << endl;
		return 1;
	}

	while( quit == false ){
		
		fps.start();

		while( SDL_PollEvent( &event ) ){
				
			//Window resize event
			if( event.type == SDL_QUIT )	quit = true;
			else if( event.type == SDL_KEYDOWN ){
				int x = 0, y = 0;
				SDL_GetMouseState( &x, &y );
				handleKeys( event.key.keysym.sym, x, y );
			}
		}
		
		update();

		render();

		if( fps.getTicks() < 1000 / FRAMES_PER_SECOND )
			SDL_Delay( 1000 / FRAMES_PER_SECOND - fps.getTicks() );
	}

	cleanUp();
		
	return 0;
}
