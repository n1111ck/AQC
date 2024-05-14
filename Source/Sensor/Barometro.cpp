#include "Sensor/Barometro.h"

namespace AQC
{
	Barometro::Barometro() :
		mAltitude(0.0)
	{
		ISensor::mpModelo = nullptr;
	}

	Barometro::~Barometro()
	{

	}

	Void
	Barometro::Simulacao(Modelo& modelo)
	{
		ISensor::mpModelo = &modelo;
	}

	Int32
	Barometro::Iniciar()
	{
		Int32 resultado = -1;

		if (ISensor::mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
			resultado = 0;
		}

		return resultado;
	}

	Void
	Barometro::Calibrar()
	{
		if (ISensor::ISensor::mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
		}
	}

	Void
	Barometro::Capturar()
	{
		if (ISensor::mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
			mAltitude = ISensor::mpModelo->Posicao().mZ;
		}
	}

	Float
	Barometro::Altitude() const
	{
		return mAltitude;
	}
}