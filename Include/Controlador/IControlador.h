#ifndef _ICONTROLADOR_H_
#define _ICONTROLADOR_H_

// Incluir Acoplador
#include "Acoplador/GerenciadorAcopladores.h"

// Incluir Sensor
#include "Sensor/GerenciadorSensores.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class IControlador
	{
	public:
		virtual Void Aplicar(const Vetor4D& referencia) = 0;
		virtual Void Capturar() = 0;
		virtual Float Frequencia() const = 0;
		virtual Vetor4D ReferenciaVelocidade() const = 0;
		virtual Vetor4D ReferenciaPosicao() const = 0;
	protected:
		GerenciadorAcopladores* mpGerenciadorAcopladores;
		GerenciadorSensores* mpGerenciadorSensores;
	};
}

#endif //_ICONTROLADOR_H_