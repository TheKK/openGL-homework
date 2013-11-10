/*

window.h

author: TheKK <thumbd03803@gmail.com>

*/

#ifndef WINDOW_H
#define WINDOW_H

#include "basicNeed.h"

enum WINDOW_TYPE{ NORMAL, OPENGL };

class Window
{
	public:
		//Constructor
		Window( char* windowTitle, int width, int height, int windowType );
	
		//Initialize	
		bool init();

		//Focus
		void focus();

		//Render
		void render();
	
		//Update
		void update();	

		//Event handler
		void eventHandler( SDL_Event e );

		//Free up data
		void free();

		//Get window's dimentions
		int getWidth();
		int getHeight();
	
		//Window focii
		bool hasMouseFocused();
		bool hasKeyboardFocused();
		bool isShown(); 
	
	private:
		//Window data
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		SDL_GLContext mContext;
		char* mWindowTitle;
		int mWindowID;
		int mWindowType;

		//Window dimensions
		int mWindowWidth;
		int mWindowHeight;
	
		//Window status
		bool mMouseFocused;
		bool mKeybroadFocused;
		bool mFullscreened;
		bool mShown;
};
#endif
