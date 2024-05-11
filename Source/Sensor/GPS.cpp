#include "GPS.h"

GPS::GPS() :
	mpModelo(nullptr),
	mLatitude(0.0),
	mLongitude(0.0)
{

}

GPS::~GPS()
{

}

Void
GPS::Simulacao(Modelo& modelo)
{
	mpModelo = &modelo;
}

Int32
GPS::Iniciar()
{
	Int32 resultado = -100;

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
GPS::Calibrar()
{
	if (mpModelo == nullptr)
	{
		// Implementacao real
	}
	else
	{
		// Implementacao simulada
		mLatitude = mpModelo->Posicao().mX;
		mLongitude = mpModelo->Posicao().mY;
	}
}

Void
GPS::Capturar()
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
GPS::Latitude() const
{
	return mLatitude;
}

Float
GPS::Longitude() const
{
	return mLongitude;
}