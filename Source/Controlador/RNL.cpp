#include "RNL.h"
#include "math.h"

RNL::RNL(
	GerenciadorAcopladores& gerAcopladores,
	GerenciadorSensores& gerSensores,
	const ParametrosModelo& parametros
):
	mMassa(parametros.mMassa),
	mGravidade(parametros.mGravidade),
	mRaio(parametros.mRaio),
	mInercia(parametros.mInercia),
	mInerciaRotacao(parametros.mInerciaRotacao),
	mPosicao({}),
	mRotacao({}),
	mVelocidadeLinear({}),
	mVelocidadeAngular({})
{
	IControlador::mpGerenciadorAcopladores = &gerAcopladores;
	IControlador::mpGerenciadorSensores = &gerSensores;

	// Calcular ganhos
	mProporcionalPosicao = ProporcionalPosicao(parametros.mConstanteTempo);
	mProporcionalVelocidade = ProporcionalVelocidade({
		parametros.mMassa,
		parametros.mInercia.mX,
		parametros.mInercia.mY,
		parametros.mInercia.mZ
	}, parametros.mConstanteTempo);
}

RNL::~RNL()
{

}

Void 
RNL::Aplicar(const Vetor4D& referencia)
{
	Vetor4D sinal;
	Vetor4D erro;
	Vetor4D sensor;

	// Capturar dados dos sensores
	Capturar();

	// Loop Posicao
	sensor = {
		mPosicao.mZ,
		mRotacao.mX,
		mRotacao.mY,
		mRotacao.mZ
	};
	erro = referencia - sensor;
	sinal = mProporcionalPosicao * erro;

	// Loop velocidade
	sensor = {
		mVelocidadeLinear.mZ,
		mVelocidadeAngular.mX,
		mVelocidadeAngular.mY,
		mVelocidadeAngular.mZ
	};
	erro = sinal - sensor;
	sinal = mProporcionalVelocidade * erro;

	// Realimentacao nao linear de multiplicacao
	// Previnir divisao por zero
	if (!(cos(mRotacao.mX) > 0) || !(cos(mRotacao.mY) > 0))
	{
		sinal.mW /= 0.01;
	}
	else
	{
		sinal.mW /= cos(mRotacao.mX) * cos(mRotacao.mY);
	}
	sinal.mY *= -1;

	// Realimentacao nao linear de soma
	sinal += RealimentacaoNaoLinear();

	IControlador::mpGerenciadorAcopladores->Aplicar(sinal);
}

Void 
RNL::Capturar()
{
	// Capturar sensores
	IControlador::mpGerenciadorSensores->Capturar();

	// Guardar informacoes
	mPosicao = IControlador::mpGerenciadorSensores->Posicao();
	mVelocidadeLinear = IControlador::mpGerenciadorSensores->VelocidadeLinear();
	mRotacao = IControlador::mpGerenciadorSensores->Rotacao();
	mVelocidadeAngular = IControlador::mpGerenciadorSensores->VelocidadeAngular();
	mDistanciaBaixo = IControlador::mpGerenciadorSensores->Baixo();
	mDistanciaFrente = IControlador::mpGerenciadorSensores->Frente();
}

Vetor4D 
RNL::ProporcionalPosicao(const Float& constanteTempo)
{
	Vetor4D resultado;

	resultado.mW = 2.0 / (27.0 * constanteTempo);
	resultado.mX = 2.0 / (27.0 * constanteTempo);
	resultado.mY = 2.0 / (27.0 * constanteTempo);
	resultado.mZ = 2.0 / (27.0 * constanteTempo);

	return resultado;
}

Vetor4D 
RNL::ProporcionalVelocidade(const Vetor4D& ganhoEstaticoInvertido, const Float& constanteTempo)
{
	Vetor4D resultado;

	resultado.mW = 1.0 / 4.0 * ganhoEstaticoInvertido.mW * 1 / constanteTempo;
	resultado.mX = 1.0 / 4.0 * ganhoEstaticoInvertido.mX * 1 / constanteTempo;
	resultado.mY = 1.0 / 4.0 * ganhoEstaticoInvertido.mY * 1 / constanteTempo;
	resultado.mZ = 1.0 / 4.0 * ganhoEstaticoInvertido.mZ * 1 / constanteTempo;

	return resultado;
}

Vetor4D
RNL::RealimentacaoNaoLinear()
{
	Vetor4D resultado;
	Float somatorioRotores = mpGerenciadorAcopladores->SomatorioRotacao();


	// Previnir divisao por zero
	if (!(cos(mRotacao.mX) > 0) || !(cos(mRotacao.mY) > 0))
	{
		resultado.mW = mMassa * mGravidade / 0.01;
	}
	else
	{
		resultado.mW = mMassa * mGravidade / (cos(mRotacao.mX) * cos(mRotacao.mY));
	}

	resultado.mX = (
		mVelocidadeAngular.mY * mVelocidadeAngular.mZ * (mInercia.mY - mInercia.mZ) -
		mVelocidadeAngular.mY * somatorioRotores * mInerciaRotacao
	);
	resultado.mY = (
		mVelocidadeAngular.mX * mVelocidadeAngular.mZ * (mInercia.mZ - mInercia.mX) +
		mVelocidadeAngular.mX * somatorioRotores * mInerciaRotacao
	);
	resultado.mZ = mVelocidadeAngular.mX * mVelocidadeAngular.mY * (mInercia.mX - mInercia.mY);

	return resultado;
}