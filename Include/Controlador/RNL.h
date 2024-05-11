#ifndef _RNL_H_
#define _RNL_H_

// Incluir Controlador
#include "IControlador.h"

// Incluir Utils
#include "Tipos.h"

class RNL : IControlador
{
	public:
		RNL();
		~RNL();

		Vetor4D Calcular();
		Void Posicao(const Vetor3D& posicao);
		Void Rotacao(const Vetor3D& rotacao);
		Void VelocidadeAngular(const Vetor3D& velocidadeAngular);
		Void VelocidadeLinear(const Vetor3D& velocidadeLinear);

	private:
		Float mMassa;
		Float mRaio;
		Vetor3D mInercia;

		Vetor3D mPosicao;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeLinear;
		Vetor3D mVelocidadeAngular;
};

#endif //_RNL_H_