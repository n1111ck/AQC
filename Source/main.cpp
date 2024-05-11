#include <iostream>

// Incluir AQC
#include "AQC.h"

// Incluir Acoplador
#include "GerenciadorAcopladores.h"	

// Incluir Controlador
#include "RNL.h"
#include "IControlador.h"

// Incluir Modelo
#include "Modelo.h"

// Incluir Sensor
#include "GerenciadorSensores.h"		

// Main file
int main()
{
	ParametrosModelo parametros;

	parametros.mMassa = 1.023;
	parametros.mRaio = 3.000e-1;
	parametros.mInercia = { 9.500e-3, 9.500e-3, 1.860e-2 };
	parametros.mPasso = 1.000e-3;
	parametros.mGravidade = 9.810;
	parametros.mInerciaRotacao = 3.788e-6;
	parametros.mRelacaoVelocidade = 2.827e+2;
	parametros.mRelacaoForca = 1.065e-5;
	parametros.mRelacaoTorque = 1.065e-7;
	parametros.mConstanteTempo = 1.316e+1;



	// Criar controlaador e gerenciadores
	RNL rnlControlador;
	GerenciadorAcopladores gerAcopladores(
		parametros.mRelacaoVelocidade,
		parametros.mRelacaoForca,
		parametros.mRelacaoTorque,
		parametros.mConstanteTempo,
		parametros.mRaio
	);
	GerenciadorSensores gerSensores;

#if _WIN32
	Modelo modelo(parametros);

	gerAcopladores.Simulacao(modelo);
	//gerSensores.Simulacao(modelo);
#endif

	//AQC<RNL> aqc(rnlControlador, gerAtuadores, gerSensores);

	Vetor4D sinal = {};
	for (UInt32 i = 0; i < 10000; i++)
	{
		if (i > 0)
		{
			gerAcopladores.Aplicar({ parametros.mMassa * parametros.mGravidade, 0, 0, 0 });
		}
		else
		{
			gerAcopladores.Aplicar({ 0, 0, 0, 0 });
		}

		std::cout << modelo.VelocidadeLinear().mZ << std::endl;
	}
}
