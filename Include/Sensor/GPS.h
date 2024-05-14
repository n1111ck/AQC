#ifndef _GPS_H_
#define _GPS_H_

// Incluir Modelo
#include "Modelo/Modelo.h"

// Incluir Sensor
#include "Sensor/ISensor.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class GPS : ISensor
	{
	public:
		GPS();
		~GPS();

		Int32 Iniciar();
		Void Calibrar();
		Void Capturar();

		Float Latitude() const;
		Float Longitude() const;

		Void Simulacao(Modelo& modelo);
	private:
		Float mLatitude;
		Float mLongitude;
	};
}

#endif //_GPS_H_