// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "j1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 j1PerfTimer::frequency = 0;

// ---------------------------------------------
j1PerfTimer::j1PerfTimer()
{
	// TODO 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
	// they are simple, one line each!

	Start();
}

// ---------------------------------------------
/**
Se llama antes del primer frame e inicializa el modulo, guarda la freqüencia de la CPU y guardamos los ticks iniciales.
*/
void j1PerfTimer::Start()
{
	//SDL_GetPerformanceCounter() returns the CPU cycles
	started_at = SDL_GetPerformanceCounter();
	frequency = SDL_GetPerformanceFrequency();
}

// ---------------------------------------------
/**
Retorna el tiempo transcurrido en milisegundos des del principio del programa. Con la variación de ticks, la freqüencia podemos saber los segundos transcurridos.
*/
double j1PerfTimer::ReadMs() const
{
	return ((SDL_GetPerformanceCounter() - started_at) / frequency) * 1000;
}

// ---------------------------------------------
/**
Retorna los ticks(ciclos de CPU) transcurridos des del principio del programa.
*/
uint64 j1PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}