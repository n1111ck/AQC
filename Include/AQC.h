#ifndef _AQC_H_
#define _AQC_H_

// Incluir Sensor
#include "GerenciadorSensores.h"

// Incluir Acoplador
#include "GerenciadorAcopladores.h"

template<typename Controlador>
class AQC
{
	public:
		AQC(
			Controlador& controlador,
			GerenciadorAcopladores& gerenciadorAcopladores,
			GerenciadorSensores& gerenciadorSensores
		);
		~AQC();

		void Run();
	private:
		Controlador* mpControlador;
		GerenciadorSensores* mpGerenciadorSensores;
		GerenciadorAcopladores* mpGerenciadorAcopladores;

		void Callback();
};

#endif // _AQC_H_