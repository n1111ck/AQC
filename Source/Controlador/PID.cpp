#include "Controlador/PID.h"
#include "math.h"

namespace AQC
{
	PID::PID()
	{

	}

	PID::PID(
		GerenciadorAcopladores& gerAcopladores,
		GerenciadorSensores& gerSensores,
		const ParametrosModelo& parametros,
		const Float& frequencia
	) : 
		mPosicao({}),
		mRotacao({}),
		mFrequencia(frequencia),
		mPidAltitude({}),
		mPidRolamento({}),
		mPidArfagem({}),
		mPidGuinada({}),
		mUltimoErro({}),
		mIntegralErro({})
	{
		IControlador::mpGerenciadorAcopladores = &gerAcopladores;
		IControlador::mpGerenciadorSensores = &gerSensores;

		// Calcular ganhos
		mPidAltitude = CalcularPidAltitude();
		mPidRolamento = CalcularPidRolamento();
		mPidArfagem = CalcularPidArfagem();
		mPidGuinada = CalcularPidGuinada();
	}

	PID::~PID()
	{

	}

	Void
	PID::Aplicar(const Vetor4D& referencia)
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
		//sinal = mProporcionalPosicao * erro;

		// Salvar calculos de integral e derivada
		mUltimoErro = erro;
		mIntegralErro += erro * 1 / mFrequencia;

		// Aplicar sinal
		IControlador::mpGerenciadorAcopladores->Aplicar(sinal);
	}

	Void
	PID::Capturar()
	{
		// Capturar sensores
		IControlador::mpGerenciadorSensores->Capturar();

		// Guardar informacoes
		mPosicao = IControlador::mpGerenciadorSensores->Posicao();
		mRotacao = IControlador::mpGerenciadorSensores->Rotacao();
	}

	Vetor3D
	PID::CalcularPidAltitude()
	{
		return Vetor3D({});
	}

	Vetor3D
	PID::CalcularPidRolamento()
	{
		return Vetor3D({});
	}

	Vetor3D
	PID::CalcularPidArfagem()
	{
		return Vetor3D({});
	}

	Vetor3D
	PID::CalcularPidGuinada()
	{
		return Vetor3D({});
	}
}