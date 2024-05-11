#ifndef _IMU_H_
#define _IMU_H_

// Incluir Modelo
#include "Modelo.h"

// Incluir Sensor
#include "ISensor.h"

// Incluir Utils
#include "Tipos.h"

class IMU : ISensor
{
	public:
		IMU();
		~IMU();

		Int32 Iniciar();
		Void Calibrar();
		Void Capturar();

		Vetor3D Rotacao() const;
		Vetor3D VelocidadeAngular() const;
		Vetor3D VelocidadeLinear() const;

		Void Simulacao(Modelo& modelo);
	private:
		Modelo* mpModelo;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeAngular;
		Vetor3D mVelocidadeLinear;
};

#endif //_IMU_H_