#include "Acoplador/GerenciadorAcopladores.h"

namespace AQC
{
	GerenciadorAcopladores::GerenciadorAcopladores()
	{

	}

	GerenciadorAcopladores::GerenciadorAcopladores(
		const Float& limiteTensao,
		const Float& relacaoVelocidade
	) :
		mpModelo(nullptr),
		mLimiteTensao(limiteTensao),
		mUltimaTensao({}),
		mRelacaoVelocidade(relacaoVelocidade)
	{
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
		mUltimaTensao = entrada;
		mUltimaTensao.Saturar(0, mLimiteTensao);

		if (mpModelo == nullptr)
		{
			// Implementacao real
		}
		else
		{
			// Implementacao simulada
			mpModelo->Aplicar(mUltimaTensao);
		}
	}

	Float
	GerenciadorAcopladores::SomatorioRotacao() const
	{
		return (
			- mUltimaTensao.mW * mRelacaoVelocidade
			+ mUltimaTensao.mX * mRelacaoVelocidade
			- mUltimaTensao.mY * mRelacaoVelocidade
			+ mUltimaTensao.mZ * mRelacaoVelocidade
			);
	}

	Vetor4D
	GerenciadorAcopladores::Tensao() const
	{
		return mUltimaTensao;
	}

}