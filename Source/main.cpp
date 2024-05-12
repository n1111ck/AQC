#include <iostream>
#include <fstream>

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
	std::ofstream csvExport("Data.csv");

	ParametrosModelo parametros;

	parametros.mMassa = 1.023;
	parametros.mRaio = 3.000e-1;
	parametros.mInercia = { 9.500e-3, 9.500e-3, 1.860e-2 };
	parametros.mPasso = 1.000e-3;
	parametros.mGravidade = 9.810;
	parametros.mInerciaRotacao = 3.788e-6;
	parametros.mRelacaoVelocidade = 7.885e+1;
	parametros.mRelacaoForca = 1.356e-05;
	parametros.mRelacaoTorque = 2.667e-07;
	parametros.mConstanteTempo = 7.600e-2;



	// Criar controlaador e gerenciadores
	//RNL rnlControlador;
	GerenciadorAcopladores gerAcopladores(
		12.0,
		parametros.mRelacaoVelocidade,
		parametros.mRelacaoForca,
		parametros.mRelacaoTorque,
		parametros.mRaio
	);
	GerenciadorSensores gerSensores;

#if _WIN32
	Modelo modelo(parametros);

	gerAcopladores.Simulacao(modelo);
	gerSensores.Simulacao(modelo);
#endif

	RNL controlador(
		gerAcopladores,
		gerSensores,
		parametros
	);

	csvExport << "Tempo[s],X[m],Y[m],Z[m],Roll[graus],Pitch[graus],Yaw[graus],M1[V],M2[V],M3[V],M4[V]" << std::endl;

	Vetor4D sinal = {};
	for (UInt32 i = 0; i < 30000; i++)
	{
		if (i > 5000)
		{
			controlador.Aplicar({ 100.0, 0, 0, 0 });
		}
		else
		{
			controlador.Aplicar({ 0, 0, 0, 0 });
		}
		modelo.Simular();

		csvExport << i * parametros.mPasso << ",";
		csvExport << modelo.VelocidadeLinear().mX << ",";
		csvExport << modelo.VelocidadeLinear().mY << ",";
		csvExport << modelo.VelocidadeLinear().mZ << ",";
		csvExport << modelo.Posicao().mX << ",";
		csvExport << modelo.Posicao().mY << ",";
		csvExport << modelo.Posicao().mZ << ",";
		csvExport << modelo.TensaoRotores().mW << ",";
		csvExport << modelo.TensaoRotores().mX << ",";
		csvExport << modelo.TensaoRotores().mY << ",";
		csvExport << modelo.TensaoRotores().mZ << std::endl;
	}

	csvExport.close();
}
