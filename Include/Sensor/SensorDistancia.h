#ifndef _SENSOR_DISTANCIA_H_
#define _SENSOR_DISTANCIA_H_

// Incluir Modelo
#include "Modelo.h"

// Incluir Sensor
#include "ISensor.h"

// Incluir Utils
#include "Tipos.h"

class SensorDistancia : ISensor
{
	public:
		SensorDistancia();
		~SensorDistancia();

		Int32 Iniciar();
		Void Calibrar();
		Void Capturar();

		Float Distancia() const;

		Void Simulacao(Modelo& modelo);
	private:
		Float mDistancia;
};

#endif //_SENSOR_DISTANCIA_H_