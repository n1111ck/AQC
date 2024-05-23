#include "Acoplador/GerenciadorAcopladores.h"

namespace AQC
{
	GerenciadorAcopladores::GerenciadorAcopladores()
	{

	}

	GerenciadorAcopladores::GerenciadorAcopladores(
		const Float& limiteTensao,
		const Float& relacaoVelocidade,
		const Float& relacaoForca,
		const Float& relacaoTorque,
		const Float& raio
	) :
		mpModelo(nullptr),
		mRelacaoVelocidade(relacaoVelocidade),
		mLimiteTensao(limiteTensao),
		mUltimaVelocidade({}),
		mUltimaTensao({})
	{
		for (UInt8 motorIndex = 0; motorIndex < 4; motorIndex++)
		{
			mpAcoplador[motorIndex] = Acoplador(
				relacaoVelocidade,
				relacaoForca,
				relacaoTorque,
				raio
			);
		}
	}

	GerenciadorAcopladores::~GerenciadorAcopladores()
	{

	}

	Void
	GerenciadorAcopladores::Simulacao(Modelo& modelo)
	{
		mpModelo = &modelo;
	}

	Void
	GerenciadorAcopladores::Aplicar(const Vetor4D& entrada)
	{
		Vetor4D tensaoRotor;

		tensaoRotor.mW = mpAcoplador[0].Calcular(entrada);
		tensaoRotor.mX = mpAcoplador[1].Calcular(entrada);
		tensaoRotor.mY = mpAcoplador[2].Calcular(entrada);
		tensaoRotor.mZ = mpAcoplador[3].Calcular(entrada);

		tensaoRotor.Saturar(0);
		tensaoRotor.Raiz();

		mUltimaVelocidade = tensaoRotor;
		tensaoRotor = tensaoRotor * (1 / mRelacaoVelocidade);
		tensaoRotor.Saturar(0, mLimiteTensao);
		mUltimaTensao = tensaoRotor;

		if (mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
			mpModelo->Aplicar(tensaoRotor);
		}
	}

	Float
	GerenciadorAcopladores::SomatorioRotacao() const
	{
		return (
			-mUltimaVelocidade.mW
			+ mUltimaVelocidade.mX
			- mUltimaVelocidade.mY
			+ mUltimaVelocidade.mZ
			);
	}

	Vetor4D
	GerenciadorAcopladores::Tensao() const
	{
		return mUltimaTensao;
	}

}