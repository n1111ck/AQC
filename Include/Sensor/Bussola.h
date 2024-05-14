#ifndef _BUSSOLA_H_
#define _BUSSOLA_H_

// Incluir Modelo
#include "Modelo/Modelo.h"

// Incluir Sensor
#include "Sensor/ISensor.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class Bussola : ISensor
	{
	public:
		Bussola();
		~Bussola();

		Int32 Iniciar();
		Void Calibrar();
		Void Capturar();

		Float Angulo() const;

		Void Simulacao(Modelo& modelo);
	private:
		Float mAngulo;
	};
}

#endif //_BUSSOLA_H_