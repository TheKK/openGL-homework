/*
timer.cpp
*/

#include "timer.h"

Timer::Timer()
{
	m_startTicks = 0;
	m_pauseTicks = 0;
	
	m_isStarted = false;
	m_isPuased = false;
}

void Timer::start()
{
	m_isStarted = true;

	m_isPuased = false;
	
	//Get the start ticks
	m_startTicks = SDL_GetTicks();
}

void Timer::pause()
{
	if( ( m_isStarted == true ) && ( m_isPuased == false ) ){
		m_isPuased = true;
	
		m_pauseTicks = SDL_GetTicks() - m_startTicks;
	}
}

void Timer::unpause()
{
	if( ( m_isPuased == true ) ){
		m_isPuased = false;
	
		m_startTicks = SDL_GetTicks() - m_pauseTicks;

		m_pauseTicks = 0;
	}
}
		
int Timer::getTicks()
{
	if( m_isStarted == true ){
		if( m_isPuased == false )	return SDL_GetTicks() - m_startTicks;
		else				return m_pauseTicks;
	}

	return 0;
}

bool Timer::isPaused()	
{ return m_isPuased; }

bool Timer::isStarted()	
{ return m_isStarted; }























	
