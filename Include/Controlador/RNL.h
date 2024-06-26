#ifndef _RNL_H_
#define _RNL_H_

// Incluir Acoplador
#include "Acoplador/GerenciadorAcopladores.h"

// Incluir Controlador
#include "Controlador/IControlador.h"

// Incluir Sensor
#include "Sensor/GerenciadorSensores.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class RNL : public IControlador
	{
	public:
		RNL();
		RNL(
			GerenciadorAcopladores& gerAcopladores,
			GerenciadorSensores& gerSensores,
			const ParametrosModelo& parametros
		);
		~RNL();

		Void Aplicar(const Vetor4D& referencia);
	private:
		// Informacoes do controlador
		Float mMassa;
		Float mGravidade;
		Float mRaio;
		Vetor3D mInercia;
		Float mInerciaRotacao;
		Vetor4D mProporcionalVelocidade;
		Vetor4D mProporcionalPosicao;

		// Informacoes dos sensores
		Vetor3D mPosicao;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeLinear;
		Vetor3D mVelocidadeAngular;

		Void Capturar();
		Vetor4D ProporcionalPosicao(const Float& constanteTempo);
		Vetor4D ProporcionalVelocidade(const Vetor4D& ganhoEstaticoInvertido, const Float& constanteTempo);
		Vetor4D RealimentacaoNaoLinear();
	};
}

#endif //_RNL_H_