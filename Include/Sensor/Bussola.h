#ifndef _BUSSOLA_H_
#define _BUSSOLA_H_

// Incluir Modelo
#include "Modelo.h"

// Incluir Sensor
#include "ISensor.h"

// Incluir Utils
#include "Tipos.h"

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

#endif //_BUSSOLA_H_