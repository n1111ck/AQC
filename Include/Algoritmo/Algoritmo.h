#ifndef _ALGORITMO_H_
#define _ALGORITMO_H_

// Incluir Sensor
#include "GerenciadorSensores.h"

// Incluir Utils
#include "Tipos.h"

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

		Void MaquinaEstados();

		Void TarefaInoperante();
		Void TarefaDecolagem();
		Void TarefaEmRota();
		Void TarefaPouso();
		Void TarefaEntrega();
		Void TarefaDesligamento();
		Void TarefaPrevencaoColisao();
};

#endif // _ALGORITMO_H_