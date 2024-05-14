#ifndef _ISENSOR_H_
#define _ISENSOR_H_

// Incluir Modelo
#include "Modelo/Modelo.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class ISensor
	{
	public:
		virtual Int32 Iniciar() = 0;
		virtual Void Calibrar() = 0;
		virtual Void Capturar() = 0;
		virtual Void Simulacao(Modelo& modelo) = 0;
	protected:
		Modelo* mpModelo;
	};
}

#endif //_ISENSOR_H_