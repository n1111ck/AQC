#ifndef _MODELO_H_
#define _MODELO_H_

// Incluir Utils
#include "Tipos.h"

class Modelo
{
	public:
		Modelo(const ParametrosModelo& parametros);
		~Modelo();

		Void Aplicar(const Vetor4D& sinal);
		Void Simular();

		Vetor3D Posicao() const;
		Vetor3D VelocidadeLinear() const;
		Vetor3D Rotacao() const;
		Vetor3D VelocidadeAngular() const;
		Vetor4D TensaoRotores() const;
	private:
		ParametrosModelo mParametros;
		Vetor4D mUltimoSinal;
		Float mTempo;

		Vetor3D mPosicao;
		Vetor3D mVelocidadeLinear;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeAngular;

		Float mSinalMotor[4];

		Float TensaoAplicada(
			const Float& constanteTempo,
			const Float& tensaoEntradaAtual,
			const Float& tensaoSaidaAtual
		) const;

		Float AceleracaoLatitude(const Float& U1, const Vetor4D& rotacao) const;
		Float AceleracaoLongitude(const Float& U1, const Vetor4D& rotacao) const;
		Float AceleracaoAltitude(const Float& U1, const Vetor4D& rotacao) const;
		Float AceleracaoRolamento(const Float& U2, const Vetor4D& velocidadeAngular, const Float& rotacaoRotorResultante) const;
		Float AceleracaoArfagem(const Float& U2, const Vetor4D& velocidadeAngular, const Float& rotacaoRotorResultante) const;
		Float AceleracaoGuinada(const Float& U2, const Vetor4D& velocidadeAngular) const;
};

#endif // _MODELO_H_