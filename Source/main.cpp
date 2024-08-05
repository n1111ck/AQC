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
		0.7,
		1 / parametros.mPasso
	);

	AQC::Algoritmo<AQC::LRE>::ParametrosAlgoritmo parametrosAlgo;
	parametrosAlgo.mAltitudeVoo = 30.0;
	parametrosAlgo.mArfagemAvanco = 5.0;
	parametrosAlgo.mToleranciaEntrega = 2.0;
	parametrosAlgo.mToleranciaPouso = 2.0;
	parametrosAlgo.mToleranciaColisao = 3.0;
	parametrosAlgo.mConstanteEquilibrio = 0.1;
	parametrosAlgo.mPasso = parametros.mPasso;


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
	csvExport << "ReferenciadZ[m/s],ReferenciadRoll[rad/s],ReferenciadPitch[rad/s],ReferenciadYaw[rad/s]" << ",";
	csvExport << "ReferenciaM1[V],ReferenciaM2[V],ReferenciaM3[V],ReferenciaM4[V]" << std::endl;

	AQC::Vetor4D sinal = {};

	// Aplicacao de chao
	modelo.Arrasto({1.2, 1.2, 1.2});
	algoritmo.NovaEntrega(-100.0, -200.0);
	AQC::Float ultimoControle = 0.0;
	AQC::Float tempoAcomodacao = 0.0;
	AQC::Boolean mColisao = false;

	for (AQC::UInt32 i = 0; i < static_cast<AQC::UInt32>(800.0 / parametros.mPasso); i++)
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
		csvExport << controlador.ReferenciaVelocidade().mZ << ",";
		csvExport << gerAcopladores.Tensao().mW << ",";
		csvExport << gerAcopladores.Tensao().mX << ",";
		csvExport << gerAcopladores.Tensao().mY << ",";
		csvExport << gerAcopladores.Tensao().mZ << std::endl;

		if (i >= 0)
		{
			//gerAcopladores.Aplicar(AQC::Vetor4D({ 5.998, 5.999, 6.000, 6.001 }));


			/*if (i < static_cast<AQC::UInt32>(1.0 / parametros.mPasso))
			{
				gerAcopladores.Aplicar(AQC::Vetor4D({ 6.0000, 6.0000, 6.0000, 6.0000 }));
			}
			else if (i < static_cast<AQC::UInt32>(2.0 / parametros.mPasso))
			{
				gerAcopladores.Aplicar(AQC::Vetor4D({ 6.0000, 6.0010, 6.0000, 5.9990 }));
			}
			else if (i < static_cast<AQC::UInt32>(3.0 / parametros.mPasso))
			{
				gerAcopladores.Aplicar(AQC::Vetor4D({ 5.9990, 6.0000, 6.0010, 6.0000 }));
			}
			else if (i < static_cast<AQC::UInt32>(4.0 / parametros.mPasso))
			{
				gerAcopladores.Aplicar(AQC::Vetor4D({ 5.9990, 6.0010, 5.9990, 6.0010 }));
			}
			else if (i < static_cast<AQC::UInt32>(5.0 / parametros.mPasso))
			{
				gerAcopladores.Aplicar(AQC::Vetor4D({ 5.9980, 5.9990, 6.0000, 6.0010 }));
			}*/

			/*if (i < static_cast<AQC::UInt32>(1.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.0, 0.0, 0.0 });
			}
			else if (i < static_cast<AQC::UInt32>(2.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.0, 0.100, 0.0 });
			}
			else if (i < static_cast<AQC::UInt32>(3.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.100, 0.100, 0.0 });
			}
			else if (i < static_cast<AQC::UInt32>(4.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.100, 0.100, 0.100 });
			}
			else if (i < static_cast<AQC::UInt32>(5.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 2.0, 0.200, 0.200, 0.200 });
			}
			else if (i < static_cast<AQC::UInt32>(6.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 2.0, 0.300, 0.300, 0.300 });
			}
			else if (i < static_cast<AQC::UInt32>(7.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.150, 0.150, 0.150 });
			}
			else if (i < static_cast<AQC::UInt32>(8.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.0, 0.0, 0.150 });
			}
			else if (i < static_cast<AQC::UInt32>(9.0 / parametros.mPasso))
			{
				controlador.Aplicar({ 1.0, 0.150, 0.0, 0.0 });
			}
			else
			{
				controlador.Aplicar({ 0.0, 0.000, 0.0, 0.000 });
			}*/

			algoritmo.Atualizar(parametros.mPasso * i);


			//modelo.Sobreposicao(modelo.Posicao().mZ < 0.0);
		}
		modelo.Simular();

		if (i > static_cast<AQC::UInt32>(100.0 / parametros.mPasso) && !mColisao && modelo.Posicao().mZ < 50.0)
		{
			modelo.Distancia(AQC::Modelo::eFrente, 2);
		}
		else if (i > static_cast<AQC::UInt32>(100.0 / parametros.mPasso))
		{
			mColisao = true;
			modelo.Distancia(AQC::Modelo::eFrente, 4);
		}

		if (i > static_cast<AQC::UInt32>(100.0 / parametros.mPasso) && modelo.Posicao().mZ < 4.0)
		{
			modelo.Distancia(AQC::Modelo::eBaixo, modelo.Posicao().mZ);
		}

		if (fabsf(0.174 - modelo.Rotacao().mY) > 0.02 * 0.174)
		{
			tempoAcomodacao = i * parametros.mPasso - 1.0;
		}

	}

	csvExport.close();
}
