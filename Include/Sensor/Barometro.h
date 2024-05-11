#ifndef _BAROMETRO_H_
#define _BAROMETRO_H_

// Incluir Modelo
#include "Modelo.h"

// Incluir Sensor
#include "ISensor.h"

// Incluir Utils
#include "Tipos.h"

class Barometro : ISensor
{
	public:
		Barometro();
		~Barometro();

		Int32 Iniciar();
		Void Calibrar();
		Void Capturar();

		Float Altitude() const;

		Void Simulacao(Modelo& modelo);
	private:
		Float mAltitude;
};

#endif //_BAROMETRO_H_