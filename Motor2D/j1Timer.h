#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

/**
Clase que controla el tiempo usando basicamente recursos de SDL.
*/
class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();				/** Se llama antes del primer frame e inicializa el modulo. */
	uint32 Read() const;		/**Retorna el tiempo transcurrido en milisegundos des del principio del programa. */
	float ReadSec() const;		/**Retorna el tiempo transcurrido en segundos des del principio del programa. */

private:
	uint32	started_at;
};

#endif //__j1TIMER_H__