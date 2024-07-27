#ifndef _LRE_H_
#define _LRE_H_

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
	class LRE : public IControlador
	{
	public:
		LRE();
		LRE(
			GerenciadorAcopladores& gerAcopladores,
			GerenciadorSensores& gerSensores,
			const ParametrosModelo& parametros,
			const Float& tempoAcomodacao,
			const Float& frequencia
		);
		~LRE();

		Void Aplicar(const Vetor4D& referencia);
		Float Frequencia() const;
		Vetor4D ReferenciaPosicao() const;
		Vetor4D ReferenciaVelocidade() const;
	private:
		// Informacoes do controlador
		Float mMassa;
		Float mGravidade;
		Float mRaio;
		Vetor3D mInercia;
		Float mInerciaRotacao;
		Float mFrequencia;
		Float mRelacaoTorque;
		Float mRelacaoForca;
		Float mRelacaoVelocidade;
		Float mConstanteTempo;
		Float mFiltro;

		// Parametros do controlador
		Vetor4D mProporcionalVelocidade;
		Vetor4D mProporcionalPosicao;
		Vetor4D mDerivativoPosicao;
		Vetor4D mUltimoErroPosicao;
		Vetor4D mUltimoSinalDerivativoPosicao;
		Vetor4D mUltimaDinamicaRotor;
		Vetor4D mUltimaReferenciaVelocidade;
		Vetor4D mUltimaReferenciaPosicao;

		// Informacoes dos sensores
		Vetor3D mPosicao;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeLinear;
		Vetor3D mVelocidadeAngular;

		Void Capturar();
		Void CalcularParametrosPosicao(const Float& tempoAcomodacao);
		Void CalcularParametrosVelocidade(const Float& tempoAcomodacao);
		Vetor4D	ControlarPlantaLinear(const Vetor4D& referencia);
		Vetor4D RealimentacaoNaoLinear();
		Vetor4D ConverterSinal(const Vetor4D& sinal);
	};
}

#endif //_RNL_H_