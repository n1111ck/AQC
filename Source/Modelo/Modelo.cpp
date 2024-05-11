#include "Modelo.h"
#include "math.h"

Modelo::Modelo(const ParametrosModelo& parametros):
	mParametros(parametros),
	mPosicao({}),
	mVelocidadeLinear({}),
	mRotacao({}),
	mVelocidadeAngular({}),
	mTempo(0.0),
	mSinalMotor{0.0, 0.0, 0.0, 0.0}
{
}

Modelo::~Modelo()
{

}

Void 
Modelo::Simular(const Vetor4D& sinal)
{
	// Variaveis para calculo
	Float k[4] = {};

	// Capturar os valores atuais
	Float sinalMotor[4] = {
		mSinalMotor[0],
		mSinalMotor[1],
		mSinalMotor[2],
		mSinalMotor[3]
	};

	// Simular Motor
	for (UInt8 motorIndex = 0; motorIndex < 4; motorIndex++)
	{
		k[0] = TensaoAplicada(mParametros.mConstanteTempo, sinal.Element(motorIndex), sinalMotor[motorIndex]);
		k[1] = TensaoAplicada(mParametros.mConstanteTempo, sinal.Element(motorIndex) + mParametros.mPasso / 2 * k[0], sinalMotor[motorIndex]);
		k[2] = TensaoAplicada(mParametros.mConstanteTempo, sinal.Element(motorIndex) + mParametros.mPasso / 2 * k[1], sinalMotor[motorIndex]);
		k[3] = TensaoAplicada(mParametros.mConstanteTempo, sinal.Element(motorIndex) + mParametros.mPasso * k[2], sinalMotor[motorIndex]);
		mSinalMotor[motorIndex] += mParametros.mPasso / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);
	}

	// Variaveis auxiliares do calculo
	Float U1 = mParametros.mRelacaoForca * (
		pow(mParametros.mRelacaoVelocidade * sinalMotor[0], 2) +
		pow(mParametros.mRelacaoVelocidade * sinalMotor[1], 2) +
		pow(mParametros.mRelacaoVelocidade * sinalMotor[2], 2) +
		pow(mParametros.mRelacaoVelocidade * sinalMotor[3], 2)
	);
	Float U2 = mParametros.mRelacaoForca * mParametros.mRaio * (
		pow(mParametros.mRelacaoVelocidade * sinalMotor[1], 2) -
		pow(mParametros.mRelacaoVelocidade * sinalMotor[3], 2)
	);
	Float U3 = mParametros.mRelacaoForca * mParametros.mRaio * (
		pow(mParametros.mRelacaoVelocidade * sinalMotor[0], 2) -
		pow(mParametros.mRelacaoVelocidade * sinalMotor[2], 2)
	);
	Float U4 = mParametros.mRelacaoTorque * (
		- pow(mParametros.mRelacaoVelocidade * sinalMotor[0], 2)
		+ pow(mParametros.mRelacaoVelocidade * sinalMotor[1], 2)
		- pow(mParametros.mRelacaoVelocidade * sinalMotor[2], 2)
		+ pow(mParametros.mRelacaoVelocidade * sinalMotor[3], 2)
	);
	Float rotacaoResultante = ( 
		- mParametros.mRelacaoVelocidade * sinalMotor[0]
		+ mParametros.mRelacaoVelocidade * sinalMotor[1]
		- mParametros.mRelacaoVelocidade * sinalMotor[2]
		+ mParametros.mRelacaoVelocidade * sinalMotor[3]
	);

	// Simular Equacao Altitude
	k[0] = AceleracaoAltitude(U1);
	k[1] = AceleracaoAltitude(U1 + mParametros.mPasso / 2 * k[0]);
	k[2] = AceleracaoAltitude(U1 + mParametros.mPasso / 2 * k[1]);
	k[3] = AceleracaoAltitude(U1 + mParametros.mPasso * k[2]);
	mVelocidadeLinear.mZ += mParametros.mPasso / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);

	// Simular Equacao Rolamento
	k[0] = AceleracaoRolamento(U2, rotacaoResultante);
	k[1] = AceleracaoRolamento(U2 + mParametros.mPasso / 2 * k[0], rotacaoResultante);
	k[2] = AceleracaoRolamento(U2 + mParametros.mPasso / 2 * k[1], rotacaoResultante);
	k[3] = AceleracaoRolamento(U2 + mParametros.mPasso * k[2], rotacaoResultante);
	mVelocidadeAngular.mX += mParametros.mPasso / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);

	// Simular Equacao Arfagem
	k[0] = AceleracaoArfagem(U3, rotacaoResultante);
	k[1] = AceleracaoArfagem(U3 + mParametros.mPasso / 2 * k[0], rotacaoResultante);
	k[2] = AceleracaoArfagem(U3 + mParametros.mPasso / 2 * k[1], rotacaoResultante);
	k[3] = AceleracaoArfagem(U3 + mParametros.mPasso * k[2], rotacaoResultante);
	mVelocidadeAngular.mY += mParametros.mPasso / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);

	// Simular Equacao Guinada
	k[0] = AceleracaoGuinada(U4);
	k[1] = AceleracaoGuinada(U4 + mParametros.mPasso / 2 * k[0]);
	k[2] = AceleracaoGuinada(U4 + mParametros.mPasso / 2 * k[1]);
	k[3] = AceleracaoGuinada(U4 + mParametros.mPasso * k[2]);
	mVelocidadeAngular.mZ += mParametros.mPasso / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);

	// Simular Equacao Latitude
	k[0] = AceleracaoLatitude(U1);
	k[1] = AceleracaoLatitude(U1 + mParametros.mPasso / 2 * k[0]);
	k[2] = AceleracaoLatitude(U1 + mParametros.mPasso / 2 * k[1]);
	k[3] = AceleracaoLatitude(U1 + mParametros.mPasso * k[2]);
	mVelocidadeLinear.mX += mParametros.mPasso / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);

	// Simular Equacao Longitude
	k[0] = AceleracaoLongitude(U1);
	k[1] = AceleracaoLongitude(U1 + mParametros.mPasso / 2 * k[0]);
	k[2] = AceleracaoLongitude(U1 + mParametros.mPasso / 2 * k[1]);
	k[3] = AceleracaoLongitude(U1 + mParametros.mPasso * k[2]);
	mVelocidadeLinear.mY += mParametros.mPasso / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);

	// Realizar integracao dos componentes posicionais
	mPosicao.mX += mVelocidadeLinear.mX * mParametros.mPasso;
	mPosicao.mY += mVelocidadeLinear.mY * mParametros.mPasso;
	mPosicao.mZ += mVelocidadeLinear.mZ * mParametros.mPasso;

	// Realizar integracao dos componentes rotacionais
	mRotacao.mX += mVelocidadeAngular.mX * mParametros.mPasso;
	mRotacao.mY += mVelocidadeAngular.mY * mParametros.mPasso;
	mRotacao.mZ += mVelocidadeAngular.mZ * mParametros.mPasso;

	// Atualizar Tempo
	mTempo += mParametros.mPasso;
}

Float
Modelo::TensaoAplicada(
	const Float& constanteTempo, 
	const Float& tensaoEntradaAtual, 
	const Float& tensaoSaidaAtual
) const
{
	return constanteTempo * (tensaoEntradaAtual - tensaoSaidaAtual);
}

Float
Modelo::AceleracaoLatitude(const Float& U1) const
{
	return U1 / mParametros.mMassa * (
		sin(mRotacao.mZ) * sin(mRotacao.mX) +
		cos(mRotacao.mZ) * sin(mRotacao.mY) * cos(mRotacao.mX)
	);
}

Float
Modelo::AceleracaoLongitude(const Float& U1) const
{
	return U1 / mParametros.mMassa * (
		- cos(mRotacao.mZ) * sin(mRotacao.mX)
		+ sin(mRotacao.mZ) * sin(mRotacao.mY) * cos(mRotacao.mX)
	);
}

Float
Modelo::AceleracaoAltitude(const Float& U1) const
{
	return -mParametros.mGravidade + cos(mRotacao.mX) * cos(mRotacao.mY) * U1 / mParametros.mMassa;
}

Float
Modelo::AceleracaoRolamento(const Float& U2, const Float& rotacaoRotorResultante) const
{
	return 1 / mParametros.mInercia.mX * (
		(mParametros.mInercia.mZ - mParametros.mInercia.mY) * mVelocidadeAngular.mY * mVelocidadeAngular.mZ +
		mParametros.mInerciaRotacao * rotacaoRotorResultante * mVelocidadeAngular.mY +
		mParametros.mRaio * U2
	);
}

Float
Modelo::AceleracaoArfagem(const Float& U3, const Float& rotacaoRotorResultante) const
{
	return 1 / mParametros.mInercia.mY * (
		(mParametros.mInercia.mX - mParametros.mInercia.mZ) * mVelocidadeAngular.mX * mVelocidadeAngular.mZ -
		mParametros.mInerciaRotacao * rotacaoRotorResultante * mVelocidadeAngular.mX +
		mParametros.mRaio * U3
	);
}

Float
Modelo::AceleracaoGuinada(const Float& U4) const
{
	return 1 / mParametros.mInercia.mZ * (
		(mParametros.mInercia.mY - mParametros.mInercia.mX) * mVelocidadeAngular.mX * mVelocidadeAngular.mY +
		mParametros.mRaio * U4
	);
}

Vetor3D 
Modelo::Posicao() const
{
	return mPosicao;
}

Vetor3D 
Modelo::VelocidadeLinear() const
{
	return mVelocidadeLinear;
}

Vetor3D 
Modelo::Rotacao() const
{
	return mRotacao;
}

Vetor3D 
Modelo::VelocidadeAngular() const
{
	return mVelocidadeAngular;
}
