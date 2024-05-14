#include "Sensor/SensorDistancia.h"

namespace AQC
{
	SensorDistancia::SensorDistancia() :
		mDistancia(4.0)
	{
		ISensor::mpModelo = nullptr;
	}

	SensorDistancia::~SensorDistancia()
	{

	}

	Void
	SensorDistancia::Simulacao(Modelo& modelo)
	{
		ISensor::mpModelo = &modelo;
	}

	Int32
	SensorDistancia::Iniciar()
	{
		Int32 resultado = -10000;

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
	SensorDistancia::Calibrar()
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
	SensorDistancia::Capturar()
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

	Float
	SensorDistancia::Distancia() const
	{
		return mDistancia;
	}
}