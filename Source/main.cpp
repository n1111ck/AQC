#include <iostream>
#include <fstream>

// Incluir Algoritmo
#include "Algoritmo/Algoritmo.h"

// Incluir Acoplador
#include "Acoplador/GerenciadorAcopladores.h"	

// Incluir Controlador
#include "Controlador/RNL.h"
#include "Controlador/IControlador.h"

// Incluir Modelo
#include "Modelo/Modelo.h"

// Incluir Sensor
#include "Sensor/GerenciadorSensores.h"		

// Main file
int main()
{
	std::ofstream csvExport("Data.csv");

	AQC::ParametrosModelo parametros;

	parametros.mMassa = 1.023;
	parametros.mRaio = 0.22225;
	parametros.mInercia = { 0.0094999, 0.0094999, 0.018576 };
	parametros.mPasso = 1.000e-3;
	parametros.mGravidade = 9.810;
	parametros.mInerciaRotacao = 3.788222486039875e-06;
	parametros.mRelacaoVelocidade = 78.841758300339833;
	parametros.mRelacaoForca = 1.355525455359016e-05;
	parametros.mRelacaoTorque = 2.667280159384542e-07;
	parametros.mConstanteTempo = 7.600e-2;

	// Criar controlaador e gerenciadores
	//RNL rnlControlador;
	AQC::GerenciadorAcopladores gerAcopladores(
		12.0,
		parametros.mRelacaoVelocidade,
		parametros.mRelacaoForca,
		parametros.mRelacaoTorque,
		parametros.mRaio
	);
	AQC::GerenciadorSensores gerSensores;

#if _WIN32
	AQC::Modelo modelo(parametros);

	gerAcopladores.Simulacao(modelo);
	gerSensores.Simulacao(modelo);
#endif

	AQC::RNL controlador(
		gerAcopladores,
		gerSensores,
		parametros
	);

	AQC::Algoritmo<AQC::RNL> algoritmo(
		controlador,
		gerSensores
	);

	csvExport << "Tempo[s],X[m],Y[m],Z[m],Roll[graus],Pitch[graus],Yaw[graus],M1[V],M2[V],M3[V],M4[V]" << std::endl;

	AQC::Vetor4D sinal = {};

	// Aplicacao de chao
	modelo.Chao(false);
	modelo.Arrasto({1.0, 1.0, 0.5});
	algoritmo.NovaEntrega(-100.0, -200.0);

	for (AQC::UInt32 i = 0; i < 1000000; i++)
	{
		algoritmo.Atualizar();
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
