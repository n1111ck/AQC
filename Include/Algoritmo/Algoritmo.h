#ifndef _ALGORITMO_H_
#define _ALGORITMO_H_

// Incluir Sensor
#include "Sensor/GerenciadorSensores.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	template<typename Controlador>
	class Algoritmo
	{
	public:

		enum eEstado
		{
			eInoperante,
			eDecolagem,
			eEmRota,
			ePouso,
			eEntrega,
			eDesligamento,
			ePrevencaoColisao
		};

		Algoritmo(
			Controlador& controlador,
			GerenciadorSensores& gerSensores
		);
		~Algoritmo();

		Void EsperarConfirmacao(Boolean valor);
		Void NovaEntrega(const Float& latitude, const Float& longitude);
		Void Atualizar();
		Void Resetar();
	private:
		Controlador* mpControlador;
		GerenciadorSensores* mpGerenciadorSensores;

		Boolean mEsperarConfirmacao;
		Boolean mIniciar;
		Boolean mEntregue;
		eEstado mEstadoAtual;

		Float mLatitudeBase;
		Float mLongitudeBase;
		Float mLatitudeDestino;
		Float mLongitudeDestino;

		Float mAltitudeVoo;
		Float mArfagemAvanco;
		Float mToleranciaEntrega;
		Float mToleranciaPouso;
		Float mToleranciaColisao;
		Float mConstanteEquilibrio;

		Vetor4D mReferencia;

		Void MaquinaEstados();
		Float Equilibrio(const Float& valor) const;

		Void TarefaInoperante();
		Void TarefaDecolagem();
		Void TarefaEmRota();
		Void TarefaPouso();
		Void TarefaEntrega();
		Void TarefaDesligamento();
		Void TarefaPrevencaoColisao();
	};
}

#endif // _ALGORITMO_H_