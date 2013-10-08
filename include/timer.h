/*
timer.h
*/
#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <SDL/SDL.h>

class Timer
{
	public:
		Timer();

		//Timer functions
		void start();
		void pause();
		void stop();
		void unpause();
		
		//Return timer ticks
		int getTicks();

		//Return timer status
		bool isPaused();
		bool isStarted();
							
	private:
		//The time when timer start
		int m_startTicks;

		//The time when timer pause
		int m_pauseTicks;

		//Show the timer status	
		bool m_isPuased;
		bool m_isStarted;
};

#endif
