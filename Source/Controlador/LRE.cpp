#include "Controlador/LRE.h"
#include "math.h"

namespace AQC
{
	LRE::LRE()
	{

	}

	LRE::LRE(
		GerenciadorAcopladores& gerAcopladores,
		GerenciadorSensores& gerSensores,
		const ParametrosModelo& parametros,
		const Float& tempoAcomodacao,
		const Float& frequencia
	) :
		mMassa(parametros.mMassa),
		mGravidade(parametros.mGravidade),
		mRaio(parametros.mRaio),
		mInercia(parametros.mInercia),
		mInerciaRotacao(parametros.mInerciaRotacao),
		mPosicao({}),
		mRotacao({}),
		mVelocidadeLinear({}),
		mVelocidadeAngular({}),
		mUltimoErroPosicao({}),
		mFrequencia(frequencia),
		mUltimaDinamicaRotor({}),
		mConstanteTempo(parametros.mConstanteTempo),
		mRelacaoVelocidade(parametros.mRelacaoVelocidade),
		mRelacaoForca(parametros.mRelacaoForca),
		mRelacaoTorque(parametros.mRelacaoTorque),
		mFiltro(frequencia*100),
		mUltimoCsi({})
	{
		IControlador::mpGerenciadorAcopladores = &gerAcopladores;
		IControlador::mpGerenciadorSensores = &gerSensores;

		// Calcular ganhos
		CalcularParametrosPosicao(tempoAcomodacao);
		CalcularParametrosVelocidade(tempoAcomodacao);
	}

	LRE::~LRE()
	{

	}

	Void
	LRE::Aplicar(const Vetor4D& referencia)
	{
		Vetor4D sinal;

		// Capturar dados dos sensores
		Capturar();

		// Controlador linear
		sinal = ControlarPlantaLinear(referencia);

		// Somar compensacao com sinal linear (-f(x) + gamma)
		sinal -= RealimentacaoNaoLinear();

		// Multiplicar pelas nao linearidades b(x)^(-1)*(-f(x) + gamma) e previnir divisao por zero
		if (!(cos(mRotacao.mX) > 0) || !(cos(mRotacao.mY) > 0))
		{
			sinal.mW /= 0.01;
		}
		else
		{
			sinal.mW *= mMassa / cos(mRotacao.mX) / cos(mRotacao.mY);
		}
		sinal.mX *= mInercia.mX;
		sinal.mY *= mInercia.mY;
		sinal.mZ *= mInercia.mZ;

		// Converter para um sinal de tensao
		sinal = ConverterSinal(sinal);

		IControlador::mpGerenciadorAcopladores->Aplicar(sinal);
	}

	Void
	LRE::Capturar()
	{
		// Capturar sensores
		IControlador::mpGerenciadorSensores->Capturar();

		// Guardar informacoes
		mPosicao = IControlador::mpGerenciadorSensores->Posicao();
		mVelocidadeLinear = IControlador::mpGerenciadorSensores->VelocidadeLinear();
		mRotacao = IControlador::mpGerenciadorSensores->Rotacao();
		mVelocidadeAngular = IControlador::mpGerenciadorSensores->VelocidadeAngular();
	}

	Void
	LRE::CalcularParametrosPosicao(const Float& tempoAcomodacao)
	{
		mProporcionalPosicao.mW = 5.0 * 1 / tempoAcomodacao;
		mProporcionalPosicao.mX = 5.0 * 1 / tempoAcomodacao;
		mProporcionalPosicao.mY = 5.0 * 1 / tempoAcomodacao;
		mProporcionalPosicao.mZ = 5.0 * 1 / tempoAcomodacao;

		mDerivativoPosicao.mW = 1.0;
		mDerivativoPosicao.mX = 1.0;
		mDerivativoPosicao.mY = 1.0;
		mDerivativoPosicao.mZ = 1.0;
	}

	Void
	LRE::CalcularParametrosVelocidade(const Float& tempoAcomodacao)
	{
		mProporcionalVelocidade.mW = 5.0 * 1 / tempoAcomodacao;
		mProporcionalVelocidade.mX = 5.0 * 1 / tempoAcomodacao;
		mProporcionalVelocidade.mY = 5.0 * 1 / tempoAcomodacao;
		mProporcionalVelocidade.mZ = 5.0 * 1 / tempoAcomodacao;
	}

	Vetor4D 
	LRE::ControlarPlantaLinear(const Vetor4D& referencia)
	{
		Vetor4D sinal;
		Vetor4D erro;
		Vetor4D sensor;
		
		//
		//	Loop Posicao
		//
		mUltimaReferenciaPosicao = referencia;
		sensor = {
			mPosicao.mZ,
			mRotacao.mX,
			mRotacao.mY,
			mRotacao.mZ
		};
		erro = referencia - sensor;
		// Calcular sinal derivativo filtrado
		mUltimoSinalDerivativoPosicao = (
			mDerivativoPosicao * (erro - mUltimoErroPosicao) * mFrequencia 
			- mUltimoSinalDerivativoPosicao * mFrequencia / mFiltro
		) * (1 / (1 + mFrequencia / mFiltro));
		sinal = mProporcionalPosicao * erro + mUltimoSinalDerivativoPosicao;
		mUltimoErroPosicao = erro;
		//sinal.Saturar({ -1.0, -10.0, -10.0, -10.0 }, { 5.0, 10.0, 10.0, 10.0 });
		mUltimaReferenciaVelocidade = sinal;

		//
		//	Loop Velocidade
		//
		sensor = {
			mVelocidadeLinear.mZ,
			mVelocidadeAngular.mX,
			mVelocidadeAngular.mY,
			mVelocidadeAngular.mZ
		};
		erro = sinal - sensor;
		sinal = mProporcionalVelocidade * erro;

		return sinal;
	}

	Vetor4D
	LRE::RealimentacaoNaoLinear()
	{
		Vetor4D resultado;
		Float somatorioRotores = mpGerenciadorAcopladores->SomatorioRotacao();

		resultado.mW = -mGravidade;
		resultado.mX = (
			mVelocidadeAngular.mY * mVelocidadeAngular.mZ * (mInercia.mZ - mInercia.mY) -
			mVelocidadeAngular.mY * somatorioRotores * mInerciaRotacao
		) / mInercia.mX;
		resultado.mY = (
			mVelocidadeAngular.mX * mVelocidadeAngular.mZ * (mInercia.mX - mInercia.mZ) +
			mVelocidadeAngular.mX * somatorioRotores * mInerciaRotacao
		) / mInercia.mY;
		resultado.mZ = (mVelocidadeAngular.mX * mVelocidadeAngular.mY * (mInercia.mY - mInercia.mX)) / mInercia.mZ;

		return resultado;
	}

	Vetor4D
	LRE::ConverterSinal(const Vetor4D& sinal)
	{
		Vetor4D resultado;

		// Conversao para velocidade dos rotores ao quadrado
		resultado.mW = Linha4D({
			1 / (4 * mRelacaoForca),
			0,
			-1 / (2 * mRaio * mRelacaoForca),
			-1 / (4 * mRelacaoTorque)
		}) * sinal;
		resultado.mX = Linha4D({
			1 / (4 * mRelacaoForca),
			1 / (2 * mRaio * mRelacaoForca),
			0,
			1 / (4 * mRelacaoTorque)
		}) * sinal;
		resultado.mY = Linha4D({
			1 / (4 * mRelacaoForca),
			0,
			1 / (2 * mRaio * mRelacaoForca),
			-1 / (4 * mRelacaoTorque)
		}) * sinal;
		resultado.mZ = Linha4D({
			1 / (4 * mRelacaoForca),
			-1 / (2 * mRaio * mRelacaoForca),
			0,
			1 / (4 * mRelacaoTorque)
		}) * sinal;

		// Converter para velocidade dos rotores em módulo
		resultado.Saturar(0.0);
		resultado.Raiz();

		// Compensar a dinâmica do rotor
		mUltimaDinamicaRotor = (
			(resultado - mUltimoCsi) * mFrequencia
			- mUltimaDinamicaRotor * mFrequencia / mFiltro
			) * (1 / (1 + mFrequencia / mFiltro) * mConstanteTempo
		);
		mUltimoCsi = resultado;
		resultado += mUltimaDinamicaRotor;
		
		// Converter para valor de tensao em cada rotor
		resultado /= mRelacaoVelocidade;

		return resultado;
	}

	Float 
	LRE::Frequencia() const
	{
		return mFrequencia;
	}

	Vetor4D
	LRE::ReferenciaVelocidade() const
	{
		return mUltimaReferenciaVelocidade;
	}

	Vetor4D
	LRE::ReferenciaPosicao() const
	{
		return mUltimaReferenciaPosicao;
	}
}

