#ifndef _ISENSOR_H_
#define _ISENSOR_H_

// Incluir Utils
#include "Tipos.h"

class ISensor
{
	public:
		virtual Int32 Iniciar() = 0;
		virtual Void Calibrar() = 0;
		virtual Void Capturar() = 0;
		virtual Void Simulacao(Modelo& modelo) = 0;
};

#endif //_ISENSOR_H_