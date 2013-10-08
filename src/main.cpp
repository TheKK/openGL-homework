/*
main.cpp
*/

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "timer.h"

using namespace std;

const short int SCREEN_WIDTH = 500;
const short int SCREEN_HEIGHT= 500;
const short int SCREEN_BPP = 32;

const short int FRAMES_PER_SECOND = 60;

SDL_Event event;

bool renderQuad = true;

bool initGL()
{
	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

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
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )	return false;

	//Create Window
	if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL );
	
	//Initialize OpenGL
	if( initGL() == false )	return false;

	//Set Caption
	SDL_WM_SetCaption( "OpenGL test", NULL );
		
	return true;
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	if( key == 'q' )	renderQuad = !renderQuad;
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
		glBegin( GL_QUADS );
			glVertex2f( -0.05f, -0.5f );
			glVertex2f(  0.05f, -0.5f );
			glVertex2f(  0.05f,  0.5f );
			glVertex2f( -0.05f,  0.5f );
		glEnd();
	}

	//Update screen
	SDL_GL_SwapBuffers();
}

void cleanUp()
{
	SDL_Quit();
}
int main( int argc, char* argv[] )
{
	bool quit = false;
	Timer fps;
	if( init() == false ){
		cout << "error" << endl;
		return 1;
	}

	while( quit == false ){
		
		fps.start();

		while( SDL_PollEvent( &event ) ){
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
