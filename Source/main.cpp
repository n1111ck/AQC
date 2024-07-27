#include <iostream>
#include <fstream>
#include <math.h>

// Incluir Algoritmo
#include "Algoritmo/Algoritmo.h"

// Incluir Acoplador
#include "Acoplador/GerenciadorAcopladores.h"	

// Incluir Controlador
#include "Controlador/LRE.h"
#include "Controlador/CascataPD.h"
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
	parametros.mPasso = 1.000e-4;
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
		parametros.mRelacaoVelocidade
	);
	AQC::GerenciadorSensores gerSensores;

#if _WIN32
	AQC::Modelo modelo(parametros);

	gerAcopladores.Simulacao(modelo);
	gerSensores.Simulacao(modelo);
#endif

	AQC::LRE controlador(
		gerAcopladores,
		gerSensores,
		parametros,
		0.4,
		1/parametros.mPasso
	);

	AQC::Algoritmo<AQC::LRE>::ParametrosAlgoritmo parametrosAlgo;
	parametrosAlgo.mAltitudeVoo = 30.0;
	parametrosAlgo.mArfagemAvanco = 5.0;
	parametrosAlgo.mToleranciaEntrega = 10.0;
	parametrosAlgo.mToleranciaPouso = 2.0;
	parametrosAlgo.mToleranciaColisao = 3.0;
	parametrosAlgo.mConstanteEquilibrio = 0.1;


	AQC::Algoritmo<AQC::LRE> algoritmo(
		controlador,
		gerSensores,
		parametrosAlgo
	);

	csvExport << "Tempo[s],";
	csvExport << "X[m],Y[m],Z[m],";
	csvExport << "Roll[rad],Pitch[rad],Yaw[rad],";
	csvExport << "dX[m/s],dY[m/s],dZ[m/s],";
	csvExport << "dRoll[rad/s],dPitch[rad/s],dYaw[rad/s],";
	csvExport << "ddX[m/s2],ddY[m/s2],ddZ[m/s2],";
	csvExport << "ddRoll[rad/s2],ddPitch[rad/s2],ddYaw[rad/s2],";
	csvExport << "M1[V],M2[V],M3[V],M4[V],";
	csvExport << "ReferenciaZ[m],ReferenciaRoll[rad],ReferenciaPitch[rad],ReferenciaYaw[rad],";
	csvExport << "ReferenciadZ[m/s],ReferenciadRoll[rad/s],ReferenciadPitch[rad/s],ReferenciadYaw[rad/s]" << std::endl;

	AQC::Vetor4D sinal = {};

	// Aplicacao de chao
	modelo.Arrasto({1.0, 1.0, 0.5});
	algoritmo.NovaEntrega(-100.0, -200.0);
	AQC::Float ultimoControle = 0.0;

	for (AQC::UInt32 i = 0; i < static_cast<AQC::UInt32>(100.0 / parametros.mPasso); i++)
	{
		csvExport << i * parametros.mPasso << ",";
		csvExport << modelo.Posicao().mX << ",";
		csvExport << modelo.Posicao().mY << ",";
		csvExport << modelo.Posicao().mZ << ",";
		csvExport << modelo.Rotacao().mX << ",";
		csvExport << modelo.Rotacao().mY << ",";
		csvExport << modelo.Rotacao().mZ << ",";
		csvExport << modelo.VelocidadeLinear().mX << ",";
		csvExport << modelo.VelocidadeLinear().mY << ",";
		csvExport << modelo.VelocidadeLinear().mZ << ",";
		csvExport << modelo.VelocidadeAngular().mX << ",";
		csvExport << modelo.VelocidadeAngular().mY << ",";
		csvExport << modelo.VelocidadeAngular().mZ << ",";
		csvExport << modelo.AceleracaoLinear().mX << ",";
		csvExport << modelo.AceleracaoLinear().mY << ",";
		csvExport << modelo.AceleracaoLinear().mZ << ",";
		csvExport << modelo.AceleracaoAngular().mX << ",";
		csvExport << modelo.AceleracaoAngular().mY << ",";
		csvExport << modelo.AceleracaoAngular().mZ << ",";
		csvExport << modelo.TensaoRotores().mW << ",";
		csvExport << modelo.TensaoRotores().mX << ",";
		csvExport << modelo.TensaoRotores().mY << ",";
		csvExport << modelo.TensaoRotores().mZ << ",";
		csvExport << controlador.ReferenciaPosicao().mW << ",";
		csvExport << controlador.ReferenciaPosicao().mX << ",";
		csvExport << controlador.ReferenciaPosicao().mY << ",";
		csvExport << controlador.ReferenciaPosicao().mZ << ",";
		csvExport << controlador.ReferenciaVelocidade().mW << ",";
		csvExport << controlador.ReferenciaVelocidade().mX << ",";
		csvExport << controlador.ReferenciaVelocidade().mY << ",";
		csvExport << controlador.ReferenciaVelocidade().mZ << std::endl;

		if (i * parametros.mPasso > 0.398)
		{
			//gerAcopladores.Aplicar(AQC::Vetor4D({ 6.0, 6.0, 6.0, 6.0 }));

			/*if (i < static_cast<AQC::UInt32>(10.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.0, 0.0, 0.0 });
			}
			else
			{
				controlador.Aplicar({ 1.0, 0.0, 0.174, 0.0 });
			}*/

			algoritmo.Atualizar();

			modelo.Sobreposicao(modelo.Posicao().mZ < 0.0);
		}
		modelo.Simular();
	}

	csvExport.close();
}
