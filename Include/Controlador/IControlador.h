#ifndef _ICONTROLADOR_H_
#define _ICONTROLADOR_H_

// Incluir Acoplador
#include "GerenciadorAcopladores.h"

// Incluir Sensor
#include "GerenciadorSensores.h"

// Incluir Utils
#include "Tipos.h"
namespace AQC
{
	class IControlador
	{
	public:
		virtual Void Aplicar(const Vetor4D& referencia) = 0;
		virtual Void Capturar() = 0;
	protected:
		GerenciadorAcopladores* mpGerenciadorAcopladores;
		GerenciadorSensores* mpGerenciadorSensores;
	};
}

#endif //_ICONTROLADOR_H_