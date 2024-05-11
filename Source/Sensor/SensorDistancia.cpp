#include "SensorDistancia.h"

SensorDistancia::SensorDistancia() :
	mpModelo(nullptr),
	mDistancia(0.0)
{

}

SensorDistancia::~SensorDistancia()
{

}

Void
SensorDistancia::Simulacao(Modelo& modelo)
{
	mpModelo = &modelo;
}

Int32
SensorDistancia::Iniciar()
{
	Int32 resultado = -10000;

	if (mpModelo == nullptr)
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
	if (mpModelo == nullptr)
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
	if (mpModelo == nullptr)
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