#ifndef _CASCATA_PD_H_
#define _CASCATA_PD_H_

// Incluir Acoplador
#include "Acoplador/GerenciadorAcopladores.h"

// Incluir Controlador
#include "Controlador/IControlador.h"

// Incluir Sensor
#include "Sensor/GerenciadorSensores.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class CascataPD : public IControlador
	{
	public:
		CascataPD();
		CascataPD(
			GerenciadorAcopladores& gerAcopladores,
			GerenciadorSensores& gerSensores,
			const ParametrosModelo& parametros,
			const Float& tempoAcomodacao,
			const Float& frequencia
		);
		~CascataPD();

		Void Aplicar(const Vetor4D& referencia);
		Float Frequencia() const;
		Vetor4D ReferenciaPosicao() const;
		Vetor4D ReferenciaVelocidade() const;
	private:
		// Parametros da planta
		ParametrosModelo mParametros;
		Float mFrequencia;
		Float mFiltro;

		// Informacoes do controlador de posicao
		Vetor4D mProporcionalVelocidade;
		Vetor4D mDerivativoVelocidade;
		Vetor4D mUltimoErroVelocidade;
		Vetor4D mUltimoSinalDerivativoVelocidade;

		// Informacoes do controlador de velocidade
		Float mProporcionalPosicao;
		Float mDerivativoPosicao;
		Vetor4D mUltimoErroPosicao;
		Vetor4D mUltimoSinalDerivativoPosicao;

		// Informacoes dos sensores
		Vetor3D mPosicao;
		Vetor3D mVelocidadeLinear;
		Vetor3D mRotacao;
		Vetor3D mVelocidadeAngular;

		// Memoria do sinal
		Vetor4D mUltimaReferenciaVelocidade;
		Vetor4D mUltimaReferenciaPosicao;
		Vetor4D mUltimoSinal;

		// Captura dados
		Void Capturar();

		// Metodos calculo
		Void CalcularParametrosPosicao(const Float& tempoAcomodacao);
		Void CalcularParametrosVelocidade(const Float& tempoAcomodacao);
		Vetor4D	ControlarPlantaLinear(const Vetor4D& referencia);
		Vetor4D ConverterSinal(const Vetor4D& sinal);
	};
}

#endif //_CASCATA_PD_H_