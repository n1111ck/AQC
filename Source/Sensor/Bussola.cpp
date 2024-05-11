#include "Bussola.h"

Bussola::Bussola() :
	mpModelo(nullptr),
	mAngulo(0.0)
{

}

Bussola::~Bussola()
{

}

Void
Bussola::Simulacao(Modelo& modelo)
{
	mpModelo = &modelo;
}

Int32 
Bussola::Iniciar()
{
	Int32 resultado = -10;

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
Bussola::Calibrar()
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
Bussola::Capturar()
{
	if (mpModelo == nullptr)
	{
		// Implementacao real
	}
	else
	{
		// Implementacao simulada
		mAngulo = mpModelo->Rotacao().mZ;
	}
}

Float
Bussola::Angulo() const
{
	return mAngulo;
}