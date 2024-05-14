#include "Sensor/Bussola.h"

namespace AQC
{
	Bussola::Bussola() :
		mAngulo(0.0)
	{
		ISensor::mpModelo = nullptr;
	}

	Bussola::~Bussola()
	{

	}

	Void
	Bussola::Simulacao(Modelo& modelo)
	{
		ISensor::mpModelo = &modelo;
	}

	Int32
	Bussola::Iniciar()
	{
		Int32 resultado = -10;

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
	Bussola::Calibrar()
	{
		if (ISensor::mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
		}
	}

	Void
	Bussola::Capturar()
	{
		if (ISensor::mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
			mAngulo = ISensor::mpModelo->Rotacao().mZ;
		}
	}

	Float
	Bussola::Angulo() const
	{
		return mAngulo;
	}
}