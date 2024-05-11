#ifndef _GPS_H_
#define _GPS_H_

// Incluir Modelo
#include "Modelo.h"

// Incluir Sensor
#include "ISensor.h"

// Incluir Utils
#include "Tipos.h"

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
		Modelo* mpModelo;
		Float mLatitude;
		Float mLongitude;
};

#endif //_GPS_H_