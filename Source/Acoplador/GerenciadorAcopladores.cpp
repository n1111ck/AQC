#include "GerenciadorAcopladores.h"

GerenciadorAcopladores::GerenciadorAcopladores(
	const Float& relacaoVelocidade,
	const Float& relacaoForca,
	const Float& relacaoTorque,
	const Float& constanteTempo,
	const Float& raio
):
	mpModelo(nullptr),
	mRelacaoVelocidade(relacaoVelocidade)
{
	for (UInt8 motorIndex = 0; motorIndex < 4; motorIndex++)
	{
		mpAcoplador[motorIndex] = Acoplador(
			relacaoVelocidade, 
			relacaoForca,
			relacaoTorque,
			constanteTempo,
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
GerenciadorAcopladores::Aplicar(const Vetor4D& entrada) const
{
	Vetor4D velocidadeRotor;

	velocidadeRotor.mW = (mpAcoplador[0].Calcular(entrada));
	velocidadeRotor.mX = (mpAcoplador[1].Calcular(entrada));
	velocidadeRotor.mY = (mpAcoplador[2].Calcular(entrada));
	velocidadeRotor.mZ = (mpAcoplador[3].Calcular(entrada));

	velocidadeRotor.Saturar(0);
	velocidadeRotor.Raiz();
	velocidadeRotor = velocidadeRotor * (1 / mRelacaoVelocidade);

	if (mpModelo == nullptr)
	{
		// Implementacao real
	}
	else
	{
		// Implementacao simulada
		mpModelo->Simular(velocidadeRotor);
	}
}
