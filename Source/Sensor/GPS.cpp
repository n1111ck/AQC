#include "GPS.h"

GPS::GPS() :
	mLatitude(0.0),
	mLongitude(0.0)
{
	ISensor::ISensor::mpModelo = nullptr;
}

GPS::~GPS()
{

}

Void
GPS::Simulacao(Modelo& modelo)
{
	ISensor::mpModelo = &modelo;
}

Int32
GPS::Iniciar()
{
	Int32 resultado = -100;

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
GPS::Calibrar()
{
	if (ISensor::mpModelo == nullptr)
	{
		// Implementacao real
	}
	else
	{
		// Implementacao simulada
		mLatitude = ISensor::mpModelo->Posicao().mX;
		mLongitude = ISensor::mpModelo->Posicao().mY;
	}
}

Void
GPS::Capturar()
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
GPS::Latitude() const
{
	return mLatitude;
}

Float
GPS::Longitude() const
{
	return mLongitude;
}