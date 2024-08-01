#include "Algoritmo/Algoritmo.h"
#include <math.h>

// Incluir Controlador
#include "Controlador/LRE.h"
#include "Controlador/CascataPD.h"

// Incluir Utils
#include "Utils/Constantes.h"

namespace AQC
{
	template<typename Controlador>
	Algoritmo<Controlador>::Algoritmo()
	{

	}

	template<typename Controlador>
	Algoritmo<Controlador>::Algoritmo(
		Controlador& controlador,
		GerenciadorSensores& gerSensores,
		const ParametrosAlgoritmo& parametrosAlgoritmo
	) :
		mpControlador(&controlador),
		mpGerenciadorSensores(&gerSensores),
		mEsperarConfirmacao(false),
		mEstadoAtual(eInoperante),
		mIniciar(false),
		mEntregue(false),
		mLatitudeBase(0.0),
		mLongitudeBase(0.0),
		mAltitudeVoo(parametrosAlgoritmo.mAltitudeVoo),
		mArfagemAvanco(parametrosAlgoritmo.mArfagemAvanco * PI / 180.0),
		mToleranciaEntrega(parametrosAlgoritmo.mToleranciaEntrega),
		mToleranciaPouso(parametrosAlgoritmo.mToleranciaPouso),
		mLatitudeDestino(0.0),
		mLongitudeDestino(0.0),
		mToleranciaColisao(parametrosAlgoritmo.mToleranciaColisao),
		mConstanteEquilibrio(parametrosAlgoritmo.mConstanteEquilibrio),
		mReferencia({}),
		mTempo(0.0),
		mTempoEntrega(-1.0),
		mTempoDecolagem(-1.0),
		mLatitudeDecolagem(0.0),
		mLongitudeDecolagem(0.0)
	{

	}

	template<typename Controlador>
	Algoritmo<Controlador>::~Algoritmo()
	{

	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::EsperarConfirmacao(Boolean valor)
	{
		mEsperarConfirmacao = valor;
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::MaquinaEstados()
	{
		switch (mEstadoAtual)
		{
		case eInoperante:
			TarefaInoperante();
			break;
		case eDecolagem:
			TarefaDecolagem();
			break;
		case eEmRota:
			TarefaEmRota();
			break;
		case ePouso:
			TarefaPouso();
			break;
		case eEntrega:
			TarefaEntrega();
			break;
		case eDesligamento:
			TarefaDesligamento();
			break;
		case ePrevencaoColisao:
			TarefaPrevencaoColisao();
			break;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::Resetar()
	{
		// Mudar estado para inoperante
		mEstadoAtual = eInoperante;

		// Resetar booleanos
		mIniciar = false;
		mEntregue = false;
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::NovaEntrega(const Float& latitude, const Float& longitude)
	{
		Resetar();
		mLatitudeDestino = latitude;
		mLongitudeDestino = longitude;
		mIniciar = true;
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaInoperante()
	{
		Vetor3D posicao = mpGerenciadorSensores->Posicao();
		// Tarefa executada
		// Inoperante

		// Condicao de transicao
		if (mIniciar)
		{
			mIniciar = false;
			mEstadoAtual = eDecolagem;
			mLatitudeBase = posicao.mX;
			mLongitudeBase = posicao.mY;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaDecolagem()
	{
		Vetor3D posicao = mpGerenciadorSensores->Posicao();

		// Condicoes iniciais
		if (mTempoDecolagem < 0.0)
		{
			mTempoDecolagem = mTempo;
			mLatitudeDecolagem = posicao.mX;
			mLongitudeDecolagem = posicao.mY;
		}

		// Tarefa executada
		mReferencia.mW = mAltitudeVoo;
		mReferencia.mX = 0.1 * Equilibrio(mLatitudeDecolagem - posicao.mX);
		mReferencia.mY = 0.1 * Equilibrio(mLongitudeDecolagem - posicao.mY);;
		mReferencia.mZ = atan2f(
			mLongitudeDestino - posicao.mY,
			mLatitudeDestino - posicao.mX
		);

		// Condicao de transicao
		if (!(mpGerenciadorSensores->Posicao().mZ < 0.9 * mAltitudeVoo))
		{
			mEstadoAtual = eEmRota;
			mTempoDecolagem = -1.0;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaEmRota()
	{
		Vetor3D posicao = mpGerenciadorSensores->Posicao();
		Float distanciaAoDestino, desvioDaRota, estimacaoLongitude;

		// Distancia ao destino
		distanciaAoDestino = pow(pow(mLongitudeDestino - posicao.mY, 2) +
			pow(mLatitudeDestino - posicao.mX, 2), 0.5);

		// Estimar longitude
		estimacaoLongitude = (mLongitudeDestino - mLongitudeBase) / (mLatitudeDestino - mLatitudeBase) * posicao.mX + mLongitudeBase;

		// Corrigir desvio da rota
		if (posicao.mY - estimacaoLongitude > 1.0)
		{
			Float multiplier = mEntregue ? -1 : 1;
			desvioDaRota = multiplier * fabsf(posicao.mY - estimacaoLongitude) / (posicao.mY - estimacaoLongitude) *
				fabsf((mLongitudeDestino - mLongitudeBase) / (mLatitudeDestino - mLatitudeBase) * mLatitudeBase + 2 * mLongitudeBase) *
				pow((mLongitudeDestino - mLongitudeBase) / (mLatitudeDestino - mLatitudeBase) + 1, 0.5);
		}
		else
		{
			desvioDaRota = 0.0;
		}

		// Tarefa executada
		mReferencia.mX = 0.0;//0.1 * Equilibrio(desvioDaRota);
		mReferencia.mY = mArfagemAvanco;
		mReferencia.mZ = atan2f(
			mLongitudeDestino - posicao.mY,
			mLatitudeDestino - posicao.mX
		);;

		// Condicao de transicao
		if (distanciaAoDestino < mToleranciaEntrega)
		{
			mEstadoAtual = ePouso;
		}
		else if (mpGerenciadorSensores->Frente() < mToleranciaColisao)
		{
			mEstadoAtual = ePrevencaoColisao;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaPouso()
	{
		Vetor3D posicao = mpGerenciadorSensores->Posicao();

		// Tarefa executada
		mReferencia.mW = 0.0;
		mReferencia.mX = 0.01 * Equilibrio(-(mLongitudeDestino - posicao.mY));
		mReferencia.mY = 0.01 * Equilibrio(-(mLatitudeDestino - posicao.mX));

		// Condicao de transicao
		if (mpGerenciadorSensores->Baixo() < mToleranciaPouso)
		{
			// Atualizar altitude para hover
			mReferencia.mW = mpGerenciadorSensores->Posicao().mZ;

			if (!mEntregue)
			{
				// Efetua entrega e prepara retorno base
				mEstadoAtual = eEntrega;
			}
			else
			{
				// Fim de operacao
				mReferencia.mW = mpGerenciadorSensores->Posicao().mZ;
				mEstadoAtual = eDesligamento;
			}
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaEntrega()
	{
		Vetor3D posicao = mpGerenciadorSensores->Posicao();

		// Primeira vez no loop
		if (mTempoEntrega < 0.0)
		{
			mTempoEntrega = mTempo;
			mAltitudeEntrega = mpGerenciadorSensores->Posicao().mZ;
			mEntregue = true;
			mLatitudeDestino = mLatitudeBase;
			mLongitudeDestino = mLongitudeBase;
			mLatitudeBase = posicao.mX;
			mLongitudeBase = posicao.mY;
		}

		// Tarefa executada
		mReferencia.mW = mAltitudeEntrega;
		mReferencia.mX = 0.1 * Equilibrio(mLongitudeBase - posicao.mY);
		mReferencia.mY = 0.1 * Equilibrio(mLatitudeBase - posicao.mX);

		// Condicao de transicao
		if (mTempo - mTempoEntrega > 10.0)
		{
			mEstadoAtual = eDecolagem;
			mTempoEntrega = -1.0;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaDesligamento()
	{
		// Tarefa executada
		mReferencia.mX = 0.0;
		mReferencia.mY = 0.0;
		Resetar();

		// Fim de Operacao
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaPrevencaoColisao()
	{
		// Tarefa executada
		// TODO: pensar num algoritmo

		// Condicao de transicao
		if (!(mpGerenciadorSensores->Frente() < mToleranciaColisao))
		{
			mEstadoAtual = eEmRota;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::Atualizar(const Float& tempo)
	{
		mTempo = tempo;
		MaquinaEstados();
		mpControlador->Aplicar(mReferencia);
	}

	template<typename Controlador>
	Float
	Algoritmo<Controlador>::Equilibrio(const Float& valor) const
	{
		return atan(mConstanteEquilibrio * valor) * 2.0 / PI;
	}

	template class Algoritmo<LRE>;
	template class Algoritmo<CascataPD>;
}