#include "Controlador/CascataPD.h"
#include "math.h"

namespace AQC
{
	CascataPD::CascataPD()
	{

	}

	CascataPD::CascataPD(
		GerenciadorAcopladores& gerAcopladores,
		GerenciadorSensores& gerSensores,
		const ParametrosModelo& parametros,
		const Float& tempoAcomodacao,
		const Float& frequencia
	) : 
		mPosicao({}),
		mVelocidadeLinear({}),
		mRotacao({}),
		mVelocidadeAngular({}),
		mFrequencia(frequencia),
		mProporcionalVelocidade({}),
		mDerivativoVelocidade({}),
		mUltimoErroVelocidade({}),
		mProporcionalPosicao(0.0),
		mDerivativoPosicao(0.0),
		mUltimoErroPosicao({}),
		mParametros(parametros),
		mUltimoSinal({}),
		mUltimaReferenciaPosicao({}),
		mUltimaReferenciaVelocidade({}),
		mFiltro(frequencia * 100)
	{
		IControlador::mpGerenciadorAcopladores = &gerAcopladores;
		IControlador::mpGerenciadorSensores = &gerSensores;

		// Calcular ganhos
		CalcularParametrosPosicao(tempoAcomodacao);
		CalcularParametrosVelocidade(tempoAcomodacao);
	}

	CascataPD::~CascataPD()
	{

	}

	Void
	CascataPD::Aplicar(const Vetor4D& referencia)
	{
		Vetor4D deltaSinal;
		Vetor4D sinal, sensor;

		// Capturar dados dos sensores
		Capturar();

		// Controlador linear
		deltaSinal = ControlarPlantaLinear(referencia);

		// Construir o sinal real (delta + anterior)
		sinal = deltaSinal + mUltimoSinal;
		mUltimoSinal = sinal;

		// Converter para um sinal de tensao
		sinal = ConverterSinal(sinal);

		// Aplicar sinal
		IControlador::mpGerenciadorAcopladores->Aplicar(sinal);
	}

	Void
	CascataPD::Capturar()
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
	CascataPD::CalcularParametrosPosicao(const Float& tempoAcomodacao)
	{
		mProporcionalPosicao = 4.0 / tempoAcomodacao;
		mDerivativoPosicao = 1.0;
	}

	Void
	CascataPD::CalcularParametrosVelocidade(const Float& tempoAcomodacao)
	{
		mProporcionalVelocidade.mW = 4.0 / tempoAcomodacao * mParametros.mMassa * mParametros.mConstanteTempo;
		mProporcionalVelocidade.mX = 4.0 / tempoAcomodacao * mParametros.mInercia.mX * mParametros.mConstanteTempo;
		mProporcionalVelocidade.mY = 4.0 / tempoAcomodacao * mParametros.mInercia.mY * mParametros.mConstanteTempo;
		mProporcionalVelocidade.mZ = 4.0 / tempoAcomodacao * mParametros.mInercia.mZ * mParametros.mConstanteTempo;

		mDerivativoVelocidade.mW = 4.0 / tempoAcomodacao * mParametros.mMassa;
		mDerivativoVelocidade.mX = 4.0 / tempoAcomodacao * mParametros.mInercia.mX;
		mDerivativoVelocidade.mY = 4.0 / tempoAcomodacao * mParametros.mInercia.mY;
		mDerivativoVelocidade.mZ = 4.0 / tempoAcomodacao * mParametros.mInercia.mZ;
	}

	Vetor4D
	CascataPD::ControlarPlantaLinear(const Vetor4D& referencia)
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
			(erro - mUltimoErroPosicao) * mFrequencia * mDerivativoPosicao
			- mUltimoSinalDerivativoPosicao * mFrequencia / mFiltro
			) * (1 / (1 + mFrequencia / mFiltro));
		sinal = erro * mProporcionalPosicao + mUltimoSinalDerivativoPosicao;
		mUltimoErroPosicao = erro;
		sinal.Saturar({ -1.0, -10.0, -10.0, -10.0 }, { 5.0, 10.0, 10.0, 10.0 });
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
		// Calcular sinal derivativo filtrado
		mUltimoSinalDerivativoVelocidade = (
			mDerivativoVelocidade * (erro - mUltimoErroVelocidade) * mFrequencia
			- mUltimoSinalDerivativoVelocidade * mFrequencia / mFiltro
			) * (1 / (1 + mFrequencia / mFiltro));
		sinal = mProporcionalVelocidade * erro + mUltimoSinalDerivativoVelocidade;
		mUltimoErroVelocidade = erro;

		return sinal;
	}

	Vetor4D
	CascataPD::ConverterSinal(const Vetor4D& sinal)
	{
		Vetor4D resultado;

		// Conversao para velocidade dos rotores ao quadrado
		resultado.mW = Linha4D({
			1 / (4 * mParametros.mRelacaoForca),
			0,
			-1 / (2 * mParametros.mRaio * mParametros.mRelacaoForca),
			-1 / (4 * mParametros.mRelacaoTorque)
			}) * sinal;
		resultado.mX = Linha4D({
			1 / (4 * mParametros.mRelacaoForca),
			1 / (2 * mParametros.mRaio * mParametros.mRelacaoForca),
			0,
			1 / (4 * mParametros.mRelacaoTorque)
			}) * sinal;
		resultado.mY = Linha4D({
			1 / (4 * mParametros.mRelacaoForca),
			0,
			1 / (2 * mParametros.mRaio * mParametros.mRelacaoForca),
			-1 / (4 * mParametros.mRelacaoTorque)
			}) * sinal;
		resultado.mZ = Linha4D({
			1 / (4 * mParametros.mRelacaoForca),
			-1 / (2 * mParametros.mRaio * mParametros.mRelacaoForca),
			0,
			1 / (4 * mParametros.mRelacaoTorque)
			}) * sinal;

		// Converter para velocidade dos rotores em módulo
		resultado.Saturar(0.0);
		resultado.Raiz();

		// Converter para valor de tensao em cada rotor
		resultado /= mParametros.mRelacaoVelocidade;

		return resultado;
	}

	Float
	CascataPD::Frequencia() const
	{
		return mFrequencia;
	}

	Vetor4D
	CascataPD::ReferenciaVelocidade() const
	{
		return mUltimaReferenciaVelocidade;
	}

	Vetor4D
	CascataPD::ReferenciaPosicao() const
	{
		return mUltimaReferenciaPosicao;
	}
}