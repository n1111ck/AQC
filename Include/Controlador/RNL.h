#ifndef _RNL_H_
#define _RNL_H_

// Incluir Acoplador
#include "GerenciadorAcopladores.h"

// Incluir Controlador
#include "IControlador.h"

// Incluir Sensor
#include "GerenciadorSensores.h"

// Incluir Utils
#include "Tipos.h"

class RNL : public IControlador
{
	public:
		RNL(
			GerenciadorAcopladores& gerAcopladores,
			GerenciadorSensores& gerSensores
		);
		~RNL();

		Void Controlar(const Vetor4D& referencia);
	private:
		Float mMassa;
		Float mRaio;
		Vetor3D mInercia;

		Vetor3D mPosicao;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeLinear;
		Vetor3D mVelocidadeAngular;
		Float mDistanciaBaixo;
		Float mDistanciaFrente;

		Void Capturar();
};

#endif //_RNL_H_