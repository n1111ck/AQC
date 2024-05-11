#include "RNL.h"

RNL::RNL():
	mInercia({}),
	mMassa(0),
	mRaio(0),
	mPosicao({}),
	mRotacao({}),
	mVelocidadeLinear({}),
	mVelocidadeAngular({})
{

}

RNL::~RNL()
{

}

Vetor4D 
RNL::Calcular()
{
	// TODO
	return Vetor4D();
}

Void 
RNL::Posicao(const Vetor3D& posicao)
{
	mPosicao = posicao;
}

Void 
RNL::Rotacao(const Vetor3D& rotacao)
{
	mRotacao = rotacao;
}

Void 
RNL::VelocidadeAngular(const Vetor3D& velocidadeAngular)
{
	mVelocidadeAngular = velocidadeAngular;
}

Void 
RNL::VelocidadeLinear(const Vetor3D& velocidadeLinear)
{
	mVelocidadeLinear = velocidadeLinear;
}