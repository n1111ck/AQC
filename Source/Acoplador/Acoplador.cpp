#include "Acoplador.h"

Acoplador::Acoplador()
{

}

Acoplador::Acoplador(
	const Float& relacaoVelocidade,
	const Float& relacaoForca,
	const Float& relacaoTorque,
	const Float& constanteTempo,
	const Float& raio
):
	mRelacaoVelocidade(relacaoVelocidade),
	mRelacaoForca(relacaoForca),
	mRelacaoTorque(relacaoTorque),
	mConstanteTempo(constanteTempo),
	mRaio(raio),
	mLinhaConversao({})
{
	switch (sMotorCounter)
	{
		case 0:
			mLinhaConversao = {
				1 / (4 * mRelacaoForca),
				0,
				1 / (2 * mRaio * mRelacaoForca),
				-1 / (4 * mRelacaoTorque)
			};
			break;
		case 1:
			mLinhaConversao = {
				1 / (4 * mRelacaoForca),
				1 / (2 * mRaio * mRelacaoForca),
				0,
				1 / (4 * mRelacaoTorque)
			};
			break;
		case 2:
			mLinhaConversao = {
				1 / (4 * mRelacaoForca),
				0,
				-1 / (2 * mRaio * mRelacaoForca),
				-1 / (4 * mRelacaoTorque)
			};
			break;
		case 3:
			mLinhaConversao = {
				1 / (4 * mRelacaoForca),
				-1 / (2 * mRaio * mRelacaoForca),
				0,
				1 / (4 * mRelacaoTorque)
			};
			break;
	}

	sMotorCounter++;
}

Acoplador::~Acoplador()
{

}

Float
Acoplador::Calcular(const Vetor4D& entrada) const
{
	return mLinhaConversao * entrada;
}

UInt8 Acoplador::sMotorCounter = 0;