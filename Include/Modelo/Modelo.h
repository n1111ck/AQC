#ifndef _MODELO_H_
#define _MODELO_H_

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class Modelo
	{
	public:

		enum eSensorDistancia
		{
			eFrente = 0,
			eBaixo
		};

		Modelo();
		Modelo(const ParametrosModelo& parametros);
		~Modelo();

		Void Aplicar(const Vetor4D& sinal);
		Void Simular();
		Void Resetar();
		Void Sobreposicao(const Boolean& valor);

		Vetor3D Posicao() const;
		Vetor3D VelocidadeLinear() const;
		Vetor3D AceleracaoLinear() const;
		Vetor3D Rotacao() const;
		Vetor3D VelocidadeAngular() const;
		Vetor3D AceleracaoAngular() const;
		Vetor4D TensaoRotores() const;
		Vetor4D VelocidadeRotores() const;
		Vetor4D RotacaoRotores() const;

		Void Posicao(const Vetor3D& posicao);
		Void VelocidadeLinear(const Vetor3D& velocidade);
		Void Rotacao(const Vetor3D& rotacao);
		Void VelocidadeAngular(const Vetor3D& velocidade);

		Void Arrasto(const Vetor3D& arrasto);

		Void Distancia(const eSensorDistancia& index, const Float& distancia);
		Float Distancia(const eSensorDistancia& index);
	private:
		ParametrosModelo mParametros;
		Vetor4D mUltimoSinal;
		Float mTempo;
		Boolean mSobreposicao;
		Vetor3D mArrasto;

		Vetor3D mPosicao;
		Vetor3D mVelocidadeLinear;
		Vetor3D mAceleracaoLinear;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeAngular;
		Vetor3D mAceleracaoAngular;

		Float mVelocidadeAngularRotor[4];
		Vetor4D mRotacaoMotor;

		Float VelocidadeAngularRotor(
			const Float& tensao,
			const Float& velocidadeAngularAtual
		) const;
		Float AceleracaoLatitude(const Float& U1, const Vetor3D& rotacao, const Vetor3D& velocidadeLinear) const;
		Float AceleracaoLongitude(const Float& U1, const Vetor3D& rotacao, const Vetor3D& velocidadeLinear) const;
		Float AceleracaoAltitude(const Float& U1, const Vetor3D& rotacao, const Vetor3D& velocidadeLinear) const;
		Float AceleracaoRolamento(const Float& U2, const Vetor3D& velocidadeAngular, const Float& rotacaoRotorResultante) const;
		Float AceleracaoArfagem(const Float& U2, const Vetor3D& velocidadeAngular, const Float& rotacaoRotorResultante) const;
		Float AceleracaoGuinada(const Float& U2, const Vetor3D& velocidadeAngular) const;

		Void AtualizarRotores();

		// Sensor de distancia
		Float mFrente;
		Float mBaixo;
	};
}

#endif // _MODELO_H_