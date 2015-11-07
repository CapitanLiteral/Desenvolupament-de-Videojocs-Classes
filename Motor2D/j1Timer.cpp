// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
/**
Se llama antes del primer frame e inicializa el modulo.
*/
void j1Timer::Start()
{
	// TODO 1: Fill Start(), Read(), ReadSec() methods
	// they are simple, one line each!
	started_at = SDL_GetTicks();;
}

// ---------------------------------------------
/**
Retorna el tiempo transcurrido en milisegundos des del principio del programa.
*/
uint32 j1Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
/**
Retorna el tiempo transcurrido en segundos des del principio del programa.
*/
float j1Timer::ReadSec() const
{
	return (SDL_GetTicks() - started_at) / 1000.0f;
}