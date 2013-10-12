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
SDL_Surface *glSurface = NULL;

GLuint myTexture = 0;
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

int ToPow2(int x)
{
    int y = 1;
    while (y < x)
        y <<= 1;
    return y;
}

GLuint LoadTexture(const char* filename)
{
    // 画像を読み込む
    SDL_Surface* surface1 = SDL_LoadBMP(filename);
    if (!surface1) {
        fprintf(stderr, "%sを読み込めませんでした: %s\n", SDL_GetError());
        return 0;
    }

    // フォーマットをRGBAに変換する
    SDL_Surface* surface2 = SDL_CreateRGBSurface(SDL_SWSURFACE, 
            ToPow2(surface1->w), ToPow2(surface1->h), 32,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if (!surface2) {
        fprintf(stderr, "変換用サーフィスを確保できませんでした: %s\n", SDL_GetError());
        SDL_FreeSurface(surface1);
        return 0;
    }
    SDL_BlitSurface(surface1, NULL, surface2, NULL);

    // テクスチャを作る
    GLuint name;
    glGenTextures(1, &name);
    glBindTexture(GL_TEXTURE_2D, name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface2->w, surface2->h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, surface2->pixels);

    // 後片付け
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface1);

    return name;
}

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

	glSurface = SDL_GetWindowSurface( glScreen );
	
	//Initialize OpenGL
	if( initGL() == false )	return false;

	//Load img
		
	return true;
}

void handleKeys( int key, int x, int y )
{
	//Toggle quad
	switch( key ){
	case SDLK_q:	renderQuad = !renderQuad;	break;
	case SDLK_ESCAPE:	quit = true;	break;
	case SDLK_x:	axis = 0;	break;
	case SDLK_y:	axis = 1;	break;
	case SDLK_z:	axis = 2;	break;
	case SDLK_1:	mode = TRANSLATION;	break;
	case SDLK_2:	mode = ROTATION;	break;
	case SDLK_3:	mode = SCALE;		break;
	case SDLK_9:	glEnable( GL_TEXTURE_2D );	break;
	case SDLK_0:	glDisable( GL_TEXTURE_2D );	break;
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

	glOrtho( -20, 20, 20, -20, 50, -50 );
}

void update()
{

}

void render()
{
	//Clear the screen	
	glClear( GL_COLOR_BUFFER_BIT );

	//Make a square background
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

	SDL_GL_SwapWindow( glScreen );
}

void cleanUp()
{	
	SDL_DestroyWindow( glScreen );
	glScreen = NULL;

	SDL_Quit();
}

int main( int argc, char* argv[] )
{
	Timer fps;
	double scaleSize[ 3 ] = { 1, 1, 1, };
	if( init() == false ){
		cout << "error" << endl;
		return 1;
	}
	
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
					case X_AXIS:	scale( cube, scaleSize[ 0 ], 1, 1 );	break;
					case Y_AXIS:	scale( cube, 1, scaleSize[ 1 ], 1 );	break;
					case Z_AXIS:	scale( cube, 1, 1, scaleSize[ 2 ] );	break;
					}
					break;
				}
			}
			if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
				resize( event.window.data1, event.window.data2 );
		}
	
		update();
		
		render();


		if( fps.getTicks() < 1000 / FRAMES_PER_SECOND )
			SDL_Delay( 1000 / FRAMES_PER_SECOND - fps.getTicks() );

	}
	cleanUp();
		
	return 0;
}
