﻿#include <iostream>
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

	// Aplicacao de chao
	modelo.Chao(false);

	for (UInt32 i = 0; i < 5000; i++)
	{
		modelo.Aplicar({ 
			6.649218051073475,
			6.650823390974534,
			6.649218051073475,
			6.647612711172417
		});
		modelo.Simular();

		csvExport << i * parametros.mPasso << ",";
		csvExport << modelo.VelocidadeAngular().mX << ",";
		csvExport << modelo.VelocidadeAngular().mY << ",";
		csvExport << modelo.VelocidadeAngular().mZ << ",";
		csvExport << modelo.Rotacao().mX << ",";
		csvExport << modelo.Rotacao().mY << ",";
		csvExport << modelo.Rotacao().mZ << ",";
		csvExport << modelo.TensaoRotores().mW << ",";
		csvExport << modelo.TensaoRotores().mX << ",";
		csvExport << modelo.TensaoRotores().mY << ",";
		csvExport << modelo.TensaoRotores().mZ << std::endl;
	}

	csvExport.close();
}
