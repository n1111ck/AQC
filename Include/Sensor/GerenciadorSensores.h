#ifndef _GERENCIADOR_SENSORES_H_
#define _GERENCIADOR_SENSORES_H_

// Incluir Modelo
#include "Modelo/Modelo.h"

// Incluir Sensor
#include "Sensor/Barometro.h"
#include "Sensor/Bussola.h"
#include "Sensor/GPS.h"
#include "Sensor/IMU.h"
#include "Sensor/SensorDistancia.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class GerenciadorSensores
	{
	public:
		GerenciadorSensores();
		~GerenciadorSensores();

		Int32 Iniciar();
		Void Calibrar();
		Void Capturar();

		Vetor3D Posicao() const;
		Vetor3D VelocidadeLinear() const;
		Vetor3D Rotacao() const;
		Vetor3D VelocidadeAngular() const;
		Float Frente() const;
		Float Baixo() const;

		Void Simulacao(Modelo& modelo);
	private:
		// Barometro
		Barometro mBarometro;
		Float mAltitude;

		// Bussola
		Bussola mBussola;
		Float mAnguloBussola;

		// GPS
		GPS mGPS;
		Float mLatitude;
		Float mLongitude;

		// IMU
		IMU mIMU;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeAngular;
		Vetor3D mVelocidadeLinear;

		// SensorDistancia
		SensorDistancia mSensorDistanciaFrente;
		Float mDistanciaFrente;
		SensorDistancia mSensorDistanciaBaixo;
		Float mDistanciaBaixo;
	};
}

#endif //_GERENCIADOR_SENSORES_H_