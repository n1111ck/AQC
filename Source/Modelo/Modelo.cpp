#include "Modelo.h"
#include "math.h"

Modelo::Modelo(const ParametrosModelo& parametros):
	mParametros(parametros),
	mPosicao({}),
	mVelocidadeLinear({}),
	mRotacao({}),
	mVelocidadeAngular({}),
	mTempo(0.0),
	mUltimoSinal({}),
	mSinalMotor{0.0, 0.0, 0.0, 0.0},
	mChao(true)
{
}

Modelo::~Modelo()
{

}

Void 
Modelo::Simular()
{
	// Variaveis para calculo
	const Float idMultiplicador[4] = {0.0, 0.5, 0.5, 1.0};
	Float kMotor[4][4] = {};
	Float kVelocidadeAltitude[4] = {};
	Float kVelocidadeRolamento[4] = {};
	Float kVelocidadeArfagem[4] = {};
	Float kVelocidadeGuinada[4] = {};
	Float kVelocidadeLatitude[4] = {};
	Float kVelocidadeLongitude[4] = {};
	Float kAltitude[4] = {};
	Float kRolamento[4] = {};
	Float kArfagem[4] = {};
	Float kGuinada[4] = {};
	Float kLatitude[4] = {};
	Float kLongitude[4] = {};
	UInt8 kId;

	// Variaveis auxiliares
	Float kU1;
	Float kU2;
	Float kU3;
	Float kU4;
	Float kRotacaoResultante;
	Vetor3D kRotacao;
	Vetor3D kVelocidadeAngular;

	for (UInt8 kIteration = 0; kIteration < 4; kIteration++)
	{
		// Previnir acesso de memoria invalido
		if (kIteration == 0)
		{
			kId = 1;
		}
		else
		{
			kId = kIteration;
		}

		// Motores
		for (UInt8 motorIndex = 0; motorIndex < 4; motorIndex++)
		{
			kMotor[motorIndex][kIteration] = TensaoAplicada(
				mUltimoSinal.Element(motorIndex) + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[motorIndex][kId - 1],
				mSinalMotor[motorIndex] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[motorIndex][kId - 1]
			);
		}

		// Calcular variaveis auxiliares
		kU1 = mParametros.mRelacaoForca * (
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1]), 2) +
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1]), 2) +
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1]), 2) +
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1]), 2)
		);
		kU2 = mParametros.mRelacaoForca * mParametros.mRaio * (
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1]), 2) -
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1]), 2)
		);
		kU3 = mParametros.mRelacaoForca * mParametros.mRaio * (
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1]), 2) -
			pow(mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1]), 2)
		);
		kU4 = mParametros.mRelacaoTorque * (
			- pow(mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1]), 2)
			+ pow(mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1]), 2)
			- pow(mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1]), 2)
			+ pow(mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1]), 2)
		);
		kRotacaoResultante = (
			- mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1])
			+ mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1])
			- mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1])
			+ mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1])
		);
		kRotacao = {
				mRotacao.mX + idMultiplicador[kIteration] * mParametros.mPasso * kRolamento[kId - 1],
				mRotacao.mY + idMultiplicador[kIteration] * mParametros.mPasso * kArfagem[kId - 1],
				mRotacao.mZ + idMultiplicador[kIteration] * mParametros.mPasso * kGuinada[kId - 1]
		};
		kVelocidadeAngular = {
				mVelocidadeAngular.mX + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeRolamento[kId - 1],
				mVelocidadeAngular.mY + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeArfagem[kId - 1],
				mVelocidadeAngular.mZ + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeGuinada[kId - 1]
		};


		// Velocidade Latitude
		kVelocidadeLatitude[kIteration] = AceleracaoLatitude(
			kU1,
			kRotacao
		);

		// Velocidade Longitude
		kVelocidadeLatitude[kIteration] = AceleracaoLongitude(
			kU1, 
			kRotacao
		);

		// Velocidade Altitude
		kVelocidadeAltitude[kIteration] = AceleracaoAltitude(
			kU1,
			kRotacao
		);

		// Velocidade Rolamento
		kVelocidadeRolamento[kIteration] = AceleracaoRolamento(
			kU2,
			kVelocidadeAngular,
			kRotacaoResultante
		);

		// Velocidade Arfagem
		kVelocidadeArfagem[kIteration] = AceleracaoArfagem(
			kU3,
			kVelocidadeAngular,
			kRotacaoResultante
		);

		// Velocidade Guinada
		kVelocidadeGuinada[kIteration] = AceleracaoGuinada(
			kU4,
			kVelocidadeAngular
		);

		// Posicao
		kLatitude[kIteration] = mVelocidadeLinear.mX + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeLatitude[kId - 1];
		kLongitude[kIteration] = mVelocidadeLinear.mY + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeLongitude[kId - 1];
		kAltitude[kIteration] = mVelocidadeLinear.mZ + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeAltitude[kId - 1];

		// Rotacao
		kRolamento[kIteration] = mVelocidadeAngular.mX + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeRolamento[kId - 1];
		kArfagem[kIteration] = mVelocidadeAngular.mY + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeArfagem[kId - 1];
		kGuinada[kIteration] = mVelocidadeAngular.mZ + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeGuinada[kId - 1];
	}
	
	// Atualizar variaveis
	for (UInt8 motorIndex = 0; motorIndex < 4; motorIndex++)
	{
		mSinalMotor[motorIndex] += mParametros.mPasso / 6.0 * (kMotor[motorIndex][0] + 2.0 * kMotor[motorIndex][1] + 2.0 * kMotor[motorIndex][2] + kMotor[motorIndex][3]);
	}
	mVelocidadeLinear.mX += mParametros.mPasso / 6.0 * (kVelocidadeLatitude[0] + 2.0 * kVelocidadeLatitude[1] + 2.0 * kVelocidadeLatitude[2] + kVelocidadeLatitude[3]);
	mVelocidadeLinear.mY += mParametros.mPasso / 6.0 * (kVelocidadeLongitude[0] + 2.0 * kVelocidadeLongitude[1] + 2.0 * kVelocidadeLongitude[2] + kVelocidadeLongitude[3]);
	mVelocidadeLinear.mZ += mParametros.mPasso / 6.0 * (kVelocidadeAltitude[0] + 2.0 * kVelocidadeAltitude[1] + 2.0 * kVelocidadeAltitude[2] + kVelocidadeAltitude[3]);
	mVelocidadeAngular.mX += mParametros.mPasso / 6.0 * (kVelocidadeRolamento[0] + 2.0 * kVelocidadeRolamento[1] + 2.0 * kVelocidadeRolamento[2] + kVelocidadeRolamento[3]);
	mVelocidadeAngular.mY += mParametros.mPasso / 6.0 * (kVelocidadeArfagem[0] + 2.0 * kVelocidadeArfagem[1] + 2.0 * kVelocidadeArfagem[2] + kVelocidadeArfagem[3]);
	mVelocidadeAngular.mZ += mParametros.mPasso / 6.0 * (kVelocidadeGuinada[0] + 2.0 * kVelocidadeGuinada[1] + 2.0 * kVelocidadeGuinada[2] + kVelocidadeGuinada[3]);
	mPosicao.mX += mParametros.mPasso / 6.0 * (kLatitude[0] + 2.0 * kLatitude[1] + 2.0 * kLatitude[2] + kLatitude[3]);
	mPosicao.mY += mParametros.mPasso / 6.0 * (kLongitude[0] + 2.0 * kLongitude[1] + 2.0 * kLongitude[2] + kLongitude[3]);
	mPosicao.mZ += mParametros.mPasso / 6.0 * (kAltitude[0] + 2.0 * kAltitude[1] + 2.0 * kAltitude[2] + kAltitude[3]);
	mRotacao.mX += mParametros.mPasso / 6.0 * (kRolamento[0] + 2.0 * kRolamento[1] + 2.0 * kRolamento[2] + kRolamento[3]);
	mRotacao.mY += mParametros.mPasso / 6.0 * (kArfagem[0] + 2.0 * kArfagem[1] + 2.0 * kArfagem[2] + kArfagem[3]);
	mRotacao.mZ += mParametros.mPasso / 6.0 * (kGuinada[0] + 2.0 * kGuinada[1] + 2.0 * kGuinada[2] + kGuinada[3]);

	// Atualizar Tempo
	mTempo += mParametros.mPasso;
}

Void
Modelo::Aplicar(const Vetor4D& sinal)
{
	mUltimoSinal = sinal;
}

Float
Modelo::TensaoAplicada(
	const Float& tensaoEntradaAtual, 
	const Float& tensaoSaidaAtual
) const
{
	return 1 / mParametros.mConstanteTempo * (tensaoEntradaAtual - tensaoSaidaAtual);
}

Float
Modelo::AceleracaoLatitude(const Float& U1, const Vetor3D& rotacao) const
{
	return U1 / mParametros.mMassa * (
		sin(rotacao.mZ) * sin(rotacao.mX) +
		cos(rotacao.mZ) * sin(rotacao.mY) * cos(rotacao.mX)
	);
}

Float
Modelo::AceleracaoLongitude(const Float& U1, const Vetor3D& rotacao) const
{
	return U1 / mParametros.mMassa * (
		- cos(rotacao.mZ) * sin(rotacao.mX)
		+ sin(rotacao.mZ) * sin(rotacao.mY) * cos(rotacao.mX)
	);
}

Float
Modelo::AceleracaoAltitude(const Float& U1, const Vetor3D& rotacao) const
{
	Float resultado = -mParametros.mGravidade + cos(rotacao.mX) * cos(rotacao.mY) * U1 / mParametros.mMassa;

	if ((mPosicao.mZ < 0.0 && resultado < 0.0) && mChao)
	{
		resultado = 0.0;
	}

	return resultado;
}

Float
Modelo::AceleracaoRolamento(const Float& U2, const Vetor3D& velocidadeAngular, const Float& rotacaoRotorResultante) const
{
	return 1 / mParametros.mInercia.mX * (
		(mParametros.mInercia.mZ - mParametros.mInercia.mY) * velocidadeAngular.mY * velocidadeAngular.mZ +
		mParametros.mInerciaRotacao * rotacaoRotorResultante * velocidadeAngular.mY +
		U2
	);
}

Float
Modelo::AceleracaoArfagem(const Float& U3, const Vetor3D& velocidadeAngular, const Float& rotacaoRotorResultante) const
{
	return 1 / mParametros.mInercia.mY * (
		(mParametros.mInercia.mX - mParametros.mInercia.mZ) * velocidadeAngular.mX * velocidadeAngular.mZ -
		mParametros.mInerciaRotacao * rotacaoRotorResultante * velocidadeAngular.mX +
		U3
	);
}

Float
Modelo::AceleracaoGuinada(const Float& U4, const Vetor3D& velocidadeAngular) const
{
	return 1 / mParametros.mInercia.mZ * (
		(mParametros.mInercia.mY - mParametros.mInercia.mX) * velocidadeAngular.mX * velocidadeAngular.mY +
		U4
	);
}

// TODO: todos getters tem que retornar valores que os sensores reais retornariam

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

Vetor4D 
Modelo::TensaoRotores() const
{
	return {
		mSinalMotor[0],
		mSinalMotor[1],
		mSinalMotor[2],
		mSinalMotor[3]
	};
}

Void
Modelo::Chao(const Boolean& valor)
{
	mChao = valor;
}