#ifndef __j1PERFTIMER_H__
#define __j1PERFTIMER_H__

#include "p2Defs.h"

/**
Clase que controla el tiempo usando la velocidad del processador.
*/
class j1PerfTimer
{
public:

	// Constructor
	j1PerfTimer();

	void Start();					/** Se llama antes del primer frame e inicializa el modulo, guarda la freqüencia de la CPU y guardamos los ticks iniciales.*/
	double ReadMs() const;			/**Retorna el tiempo transcurrido en milisegundos des del principio del programa. Con la variación de ticks, la freqüencia podemos saber los segundos transcurridos.*/
	uint64 ReadTicks() const;		/**Retorna los ticks(ciclos de CPU) transcurridos des del principio del programa. */

private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //__j1PERFTIMER_H__