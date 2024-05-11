#ifndef _MUTEX_H_
#define _MUTEX_H_

// Incluir Utils
#include "Tipos.h"

class Mutex
{
	public:
		Mutex();
		~Mutex();

		Void Travar();
		Boolean TentarTravar();
		Void Destravar();
	private:
		enum Estado
		{
			DESTRAVADO = 0,
			TRAVADO
		};

		Estado mEstado;
};

#endif //_MUTEX_H_