#include "Sensor/IMU.h"

namespace AQC
{
	IMU::IMU() :
		mRotacao({}),
		mVelocidadeAngular({}),
		mVelocidadeLinear({})
	{
		ISensor::mpModelo = nullptr;
	}

	IMU::~IMU()
	{

	}

	Void
	IMU::Simulacao(Modelo& modelo)
	{
		ISensor::mpModelo = &modelo;
	}

	Int32
	IMU::Iniciar()
	{
		Int32 resultado = -1000;

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
	IMU::Calibrar()
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
	IMU::Capturar()
	{
		if (ISensor::mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
			mRotacao = ISensor::mpModelo->Rotacao();
			mVelocidadeAngular = ISensor::mpModelo->VelocidadeAngular();
			mVelocidadeLinear = ISensor::mpModelo->VelocidadeLinear();
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
}