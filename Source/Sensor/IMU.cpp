#include "IMU.h"

IMU::IMU() :
	mpModelo(nullptr),
	mRotacao({}),
	mVelocidadeAngular({}),
	mVelocidadeLinear({})
{

}

IMU::~IMU()
{

}

Void
IMU::Simulacao(Modelo& modelo)
{
	mpModelo = &modelo;
}

Int32
IMU::Iniciar()
{
	Int32 resultado = -1000;

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
IMU::Calibrar()
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
IMU::Capturar()
{
	if (mpModelo == nullptr)
	{
		// Implementacao real
	}
	else
	{
		// Implementacao simulada
		mRotacao = mpModelo->Rotacao();
		mVelocidadeAngular = mpModelo->VelocidadeAngular();
		mVelocidadeLinear = mpModelo->VelocidadeLinear();
	}
}

Vetor3D
IMU::Rotacao() const
{
	return mRotacao;
}

Vetor3D
IMU::VelocidadeAngular() const
{
	return mVelocidadeAngular;
}

Vetor3D
IMU::VelocidadeLinear() const
{
	return mVelocidadeLinear;
}