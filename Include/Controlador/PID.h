#ifndef _PID_H_
#define _PID_H_

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
	class PID : public IControlador
	{
	public:
		PID();
		PID(
			GerenciadorAcopladores& gerAcopladores,
			GerenciadorSensores& gerSensores,
			const ParametrosModelo& parametros,
			const Float& frequencia
		);
		~PID();

		Void Aplicar(const Vetor4D& referencia);
	private:
		// Informacoes do controlador
		Vetor3D mPidAltitude;
		Vetor3D mPidRolamento;
		Vetor3D mPidArfagem;
		Vetor3D mPidGuinada;
		Vetor4D mUltimoErro;
		Vetor4D mIntegralErro;
		Float mFrequencia;

		// Informacoes dos sensores
		Vetor3D mPosicao;
		Vetor3D mRotacao;

		// Captura dados
		Void Capturar();

		// Metodos calculo
		Vetor3D CalcularPidAltitude();
		Vetor3D CalcularPidRolamento();
		Vetor3D CalcularPidArfagem();
		Vetor3D CalcularPidGuinada();
	};
}

#endif //_PID_H_