#include "Modelo/Modelo.h"
#include "math.h"

// Incluir Utils
#include "Utils/Constantes.h"

namespace AQC
{
	Modelo::Modelo()
	{

	}

	Modelo::Modelo(const ParametrosModelo& parametros) :
		mParametros(parametros),
		mPosicao({}),
		mVelocidadeLinear({}),
		mRotacao({}),
		mVelocidadeAngular({}),
		mTempo(0.0),
		mUltimoSinal({}),
		mSinalMotor{ 0.0, 0.0, 0.0, 0.0 },
		mSobreposicao(false),
		mArrasto({})
	{
	}

	Modelo::~Modelo()
	{

	}

	Void
	Modelo::Simular()
	{
		// Variaveis para calculo
		const Float idMultiplicador[4] = { 0.0, 0.5, 0.5, 1.0 };
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
		Float aU1;
		Float aU2;
		Float aU3;
		Float aU4;
		Float aRotacaoResultante;
		Vetor3D aRotacao;
		Vetor3D aVelocidadeAngular;
		Vetor3D aVelocidadeLinear;

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
			aU1 = mParametros.mRelacaoForca * (
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1]), 2) +
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1]), 2) +
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1]), 2) +
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1]), 2)
				);
			aU2 = mParametros.mRelacaoForca * mParametros.mRaio * (
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1]), 2) -
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1]), 2)
				);
			aU3 = mParametros.mRelacaoForca * mParametros.mRaio * (
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1]), 2) -
				pow(mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1]), 2)
				);
			aU4 = mParametros.mRelacaoTorque * (
				-pow(mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1]), 2)
				+ pow(mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1]), 2)
				- pow(mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1]), 2)
				+ pow(mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1]), 2)
				);
			aRotacaoResultante = (
				-mParametros.mRelacaoVelocidade * (mSinalMotor[0] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[0][kId - 1])
				+ mParametros.mRelacaoVelocidade * (mSinalMotor[1] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[1][kId - 1])
				- mParametros.mRelacaoVelocidade * (mSinalMotor[2] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[2][kId - 1])
				+ mParametros.mRelacaoVelocidade * (mSinalMotor[3] + idMultiplicador[kIteration] * mParametros.mPasso * kMotor[3][kId - 1])
				);
			aRotacao = {
					mRotacao.mX + idMultiplicador[kIteration] * mParametros.mPasso * kRolamento[kId - 1],
					mRotacao.mY + idMultiplicador[kIteration] * mParametros.mPasso * kArfagem[kId - 1],
					mRotacao.mZ + idMultiplicador[kIteration] * mParametros.mPasso * kGuinada[kId - 1]
			};
			aVelocidadeAngular = {
					mVelocidadeAngular.mX + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeRolamento[kId - 1],
					mVelocidadeAngular.mY + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeArfagem[kId - 1],
					mVelocidadeAngular.mZ + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeGuinada[kId - 1]
			};
			aVelocidadeLinear = {
					mVelocidadeLinear.mX + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeLatitude[kId - 1],
					mVelocidadeLinear.mY + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeLongitude[kId - 1],
					mVelocidadeLinear.mZ + idMultiplicador[kIteration] * mParametros.mPasso * kVelocidadeAltitude[kId - 1]
			};


			// Velocidade Latitude
			kVelocidadeLatitude[kIteration] = AceleracaoLatitude(
				aU1,
				aRotacao,
				aVelocidadeLinear
			);

			// Velocidade Longitude
			kVelocidadeLongitude[kIteration] = AceleracaoLongitude(
				aU1,
				aRotacao,
				aVelocidadeLinear
			);

			// Velocidade Altitude
			kVelocidadeAltitude[kIteration] = AceleracaoAltitude(
				aU1,
				aRotacao,
				aVelocidadeLinear
			);

			// Velocidade Rolamento
			kVelocidadeRolamento[kIteration] = AceleracaoRolamento(
				aU2,
				aVelocidadeAngular,
				aRotacaoResultante
			);

			// Velocidade Arfagem
			kVelocidadeArfagem[kIteration] = AceleracaoArfagem(
				aU3,
				aVelocidadeAngular,
				aRotacaoResultante
			);

			// Velocidade Guinada
			kVelocidadeGuinada[kIteration] = AceleracaoGuinada(
				aU4,
				aVelocidadeAngular
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
		AtualizarRotores();

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
	Modelo::AceleracaoLatitude(const Float& U1, const Vetor3D& rotacao, const Vetor3D& velocidadeLinear) const
	{
		return U1 / mParametros.mMassa * (
			sin(rotacao.mZ) * sin(rotacao.mX) +
			cos(rotacao.mZ) * sin(rotacao.mY) * cos(rotacao.mX)
			) - mArrasto.mX * velocidadeLinear.mX;
	}

	Float
	Modelo::AceleracaoLongitude(const Float& U1, const Vetor3D& rotacao, const Vetor3D& velocidadeLinear) const
	{
		return U1 / mParametros.mMassa * (
			-cos(rotacao.mZ) * sin(rotacao.mX)
			+ sin(rotacao.mZ) * sin(rotacao.mY) * cos(rotacao.mX)
			) - mArrasto.mY * velocidadeLinear.mY;
	}

	Float
	Modelo::AceleracaoAltitude(const Float& U1, const Vetor3D& rotacao, const Vetor3D& velocidadeLinear) const
	{
		Float resultado = -mParametros.mGravidade
			+ cos(rotacao.mX) * cos(rotacao.mY) * U1 / mParametros.mMassa
			- mArrasto.mZ * velocidadeLinear.mZ;

		if (resultado < 0.0 && mSobreposicao)
		{
			resultado = 0.0;
		}

		return resultado;
	}

	Float
	Modelo::AceleracaoRolamento(const Float& U2, const Vetor3D& velocidadeAngular, const Float& rotacaoRotorResultante) const
	{
		return 1 / mParametros.mInercia.mX * (
			(mParametros.mInercia.mZ - mParametros.mInercia.mY) * velocidadeAngular.mY * velocidadeAngular.mZ -
			mParametros.mInerciaRotacao * rotacaoRotorResultante * velocidadeAngular.mY +
			U2
		);
	}

	Float
	Modelo::AceleracaoArfagem(const Float& U3, const Vetor3D& velocidadeAngular, const Float& rotacaoRotorResultante) const
	{
		return 1 / mParametros.mInercia.mY * (
			(mParametros.mInercia.mX - mParametros.mInercia.mZ) * velocidadeAngular.mX * velocidadeAngular.mZ +
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

	Void
	Modelo::Posicao(const Vetor3D& posicao)
	{
		mPosicao = posicao;
	}

	Vetor3D
	Modelo::VelocidadeLinear() const
	{
		return mVelocidadeLinear;
	}

	Void
	Modelo::VelocidadeLinear(const Vetor3D& velocidade)
	{
		mVelocidadeLinear = velocidade;
	}

	Vetor3D
	Modelo::Rotacao() const
	{
		return mRotacao;
	}

	Void
	Modelo::Rotacao(const Vetor3D& rotacao)
	{
		mRotacao = rotacao;
	}

	Vetor3D
	Modelo::VelocidadeAngular() const
	{
		return mVelocidadeAngular;
	}

	Void
	Modelo::VelocidadeAngular(const Vetor3D& velocidade)
	{
		mVelocidadeAngular = velocidade;
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
	Modelo::Arrasto(const Vetor3D& arrasto)
	{
		mArrasto = arrasto;
	}

	Void
	Modelo::Resetar()
	{
		mUltimoSinal = Vetor4D();
		mTempo = 0.0;
		mPosicao = Vetor3D();
		mVelocidadeLinear = Vetor3D();
		mRotacao = Vetor3D();
		mVelocidadeAngular = Vetor3D();
		mSinalMotor[0] = 0.0;
		mSinalMotor[1] = 0.0;
		mSinalMotor[2] = 0.0;
		mSinalMotor[3] = 0.0;
		mRotacaoMotor = Vetor4D();
	}

	Void 
	Modelo::Sobreposicao(const Boolean& valor)
	{
		mSobreposicao = valor;
	}

	Vetor4D 
	Modelo::VelocidadeRotores() const
	{
		return {
			mParametros.mRelacaoVelocidade * mSinalMotor[0],
			mParametros.mRelacaoVelocidade * mSinalMotor[1],
			mParametros.mRelacaoVelocidade * mSinalMotor[2],
			mParametros.mRelacaoVelocidade * mSinalMotor[3]
		};
	}

	Void
	Modelo::AtualizarRotores()
	{
		Vetor4D velocidadeRotores = VelocidadeRotores();
		Vetor4D rotacao = mRotacaoMotor;

		rotacao.mW += velocidadeRotores.mW * mParametros.mPasso;
		rotacao.mX += velocidadeRotores.mX * mParametros.mPasso;
		rotacao.mY += velocidadeRotores.mY * mParametros.mPasso;
		rotacao.mZ += velocidadeRotores.mZ * mParametros.mPasso;

		rotacao.mW -= 2 * PI * floor(rotacao.mW / (2 * PI));
		rotacao.mX -= 2 * PI * floor(rotacao.mX / (2 * PI));
		rotacao.mY -= 2 * PI * floor(rotacao.mY / (2 * PI));
		rotacao.mZ -= 2 * PI * floor(rotacao.mZ / (2 * PI));

		mRotacaoMotor = rotacao;
	}

	Vetor4D 
	Modelo::RotacaoRotores() const
	{
		return mRotacaoMotor;
	}


}