/*

window.cpp

author: TheKK <thumbd03803@gmail.com>

*/

#include "window.h"

Window::Window( char* windowTitle, int width, int height, int windowType )
{
	mWindowWidth = width;
	mWindowHeight = height;
	mWindowTitle = windowTitle;
	mWindowType = windowType;

	mWindowID = -1;

	mWindowWidth = 0;
	mWindowHeight = 0;

	mMouseFocused = false;
	mKeybroadFocused = false;
	mFullscreened = false;
	mShown = false;	
}

bool Window::init()
{
	//Create window
	switch( mWindowType ){
	case NORMAL:
		mWindow = SDL_CreateWindow(
				mWindowTitle,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				mWindowWidth, mWindowHeight,
				SDL_WINDOW_SHOWN
				);
		mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED );
		break;
	case OPENGL:
		mWindow = SDL_CreateWindow(
				mWindowTitle,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				mWindowWidth, mWindowHeight,
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
				);
		mContext = SDL_GL_CreateContext( mWindow );
		break;	
	}
	mWindowID = SDL_GetWindowID( mWindow );

	if( mWindow == NULL || mRenderer == NULL || mWindowID == -1 )
		return false;
	else
		return true;
}

void Window::focus()
{
}

void Window::render()
{
}
	
void Window::update()
{
}

void Window::eventHandler( SDL_Event e )
{
}

void Window::free()
{
	//Free window
	SDL_DestroyWindow( mWindow );	
	mWindow = NULL;
	
	SDL_DestroyRenderer( mRenderer );
	mRenderer = NULL;
	
	SDL_GL_DeleteContext( mContext );
}

int Window::getWidth()
{
	return mWindowWidth;
}
	
int Window::getHeight()
{
	return mWindowHeight;
}

bool Window::hasMouseFocused()
{
	return mMouseFocused;
}

bool Window::hasKeyboardFocused()
{
	return mKeybroadFocused;
}

bool Window::isShown()
{
	return mShown;
}
