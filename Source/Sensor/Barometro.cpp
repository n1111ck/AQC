#include "Barometro.h"

Barometro::Barometro() :
	mpModelo(nullptr),
	mAltitude(0.0)
{

}

Barometro::~Barometro()
{

}

Void
Barometro::Simulacao(Modelo& modelo)
{
	mpModelo = &modelo;
}

Int32
Barometro::Iniciar()
{
	Int32 resultado = -1;

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
Barometro::Calibrar()
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
Barometro::Capturar()
{
	if (mpModelo == nullptr)
	{
		// Implementacao real
	}
	else
	{
		// Implementacao simulada
		mAltitude = mpModelo->Posicao().mZ;
	}
}

Float
Barometro::Altitude() const
{
	return mAltitude;
}
