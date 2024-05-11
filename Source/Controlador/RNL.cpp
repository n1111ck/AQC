#include "RNL.h"

RNL::RNL(
	GerenciadorAcopladores& gerAcopladores,
	GerenciadorSensores& gerSensores
):
	mInercia({}),
	mMassa(0),
	mRaio(0),
	mPosicao({}),
	mRotacao({}),
	mVelocidadeLinear({}),
	mVelocidadeAngular({})
{
	IControlador::mpGerenciadorAcopladores = &gerAcopladores;
	IControlador::mpGerenciadorSensores = &gerSensores;
}

RNL::~RNL()
{

}

Void 
RNL::Controlar(const Vetor4D& referencia)
{
	Vetor4D sinal;
	Capturar();

	// Implementacao do controlador

	IControlador::mpGerenciadorAcopladores->Aplicar(sinal);
}

Void 
RNL::Capturar()
{
	mPosicao = IControlador::mpGerenciadorSensores->Posicao();
	mVelocidadeLinear = IControlador::mpGerenciadorSensores->VelocidadeLinear();
	mRotacao = IControlador::mpGerenciadorSensores->Rotacao();
	mVelocidadeAngular = IControlador::mpGerenciadorSensores->VelocidadeAngular();
	mDistanciaBaixo = IControlador::mpGerenciadorSensores->Baixo();
	mDistanciaFrente = IControlador::mpGerenciadorSensores->Frente();
}