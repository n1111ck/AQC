#include "Algoritmo/Algoritmo.h"
#include <math.h>

// Incluir Controlador
#include "Controlador/RNL.h"

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
		GerenciadorSensores& gerSensores
	) :
		mpControlador(&controlador),
		mpGerenciadorSensores(&gerSensores),
		mEsperarConfirmacao(false),
		mEstadoAtual(eInoperante),
		mIniciar(false),
		mEntregue(false),
		mLatitudeBase(0.0),
		mLongitudeBase(0.0),
		mAltitudeVoo(200.0),
		mArfagemAvanco(5 * PI / 180.0),
		mToleranciaEntrega(10.0),
		mToleranciaPouso(2.0),
		mLatitudeDestino(0.0),
		mLongitudeDestino(0.0),
		mToleranciaColisao(3.0),
		mConstanteEquilibrio(0.1),
		mReferencia({})
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

		// Tarefa executada
		mReferencia.mW = mAltitudeVoo;
		mReferencia.mX = 0.0;
		mReferencia.mY = 0.0;
		mReferencia.mZ = atan2f(
			mLongitudeDestino - posicao.mY,
			mLatitudeDestino - posicao.mX
		);

		// Condicao de transicao
		if (!(mpGerenciadorSensores->Posicao().mZ < 0.9 * mAltitudeVoo))
		{
			mEstadoAtual = eEmRota;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaEmRota()
	{
		Vetor3D posicao = mpGerenciadorSensores->Posicao();
		Float distanciaAoDestino;

		if (
			fabs(mLatitudeDestino - mLatitudeBase) > fabs(posicao.mX - mLatitudeBase) ||
			fabs(mLongitudeDestino - mLongitudeBase) > fabs(posicao.mY - mLongitudeBase)
			)
		{
			distanciaAoDestino = pow(pow(mLongitudeDestino - posicao.mY, 2) +
				pow(mLatitudeDestino - posicao.mX, 2), 0.5);
		}
		else
		{
			distanciaAoDestino = -pow(pow(mLongitudeDestino - posicao.mY, 2) +
				pow(mLatitudeDestino - posicao.mX, 2), 0.5);
		}

		// TODO: fazer a correcao por rolamento
		// 
		// Tarefa executada
		mReferencia.mX = 0.0;
		mReferencia.mY = mArfagemAvanco * Equilibrio(distanciaAoDestino);
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
		Float distanciaAoDestino;

		if (
			fabs(mLatitudeDestino - mLatitudeBase) > fabs(posicao.mX - mLatitudeBase) ||
			fabs(mLongitudeDestino - mLongitudeBase) > fabs(posicao.mY - mLongitudeBase)
			)
		{
			distanciaAoDestino = pow(pow(mLongitudeDestino - posicao.mY, 2) +
				pow(mLatitudeDestino - posicao.mX, 2), 0.5);
		}
		else
		{
			distanciaAoDestino = -pow(pow(mLongitudeDestino - posicao.mY, 2) +
				pow(mLatitudeDestino - posicao.mX, 2), 0.5);
		}

		// Tarefa executada
		mReferencia.mW = 0.0;
		mReferencia.mX = 0.0;
		mReferencia.mY = mArfagemAvanco * Equilibrio(distanciaAoDestino);

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
				mEstadoAtual = eDesligamento;
			}
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaEntrega()
	{
		// TODO: decidir o que fazer na entrega
		Float latitudeAtual = mLatitudeDestino, longitudeAtual = mLongitudeDestino;

		// Tarefa executada
		mEntregue = true;
		mLatitudeDestino = mLatitudeBase;
		mLongitudeDestino = mLongitudeBase;
		mLatitudeBase = latitudeAtual;
		mLongitudeBase = longitudeAtual;

		// Condicao de transicao
		if (true)
		{
			mEstadoAtual = eDecolagem;
		}
	}

	template<typename Controlador>
	Void
	Algoritmo<Controlador>::TarefaDesligamento()
	{
		// Tarefa executada
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
	Algoritmo<Controlador>::Atualizar()
	{
		MaquinaEstados();
		mpControlador->Aplicar(mReferencia);
	}

	template<typename Controlador>
	Float
	Algoritmo<Controlador>::Equilibrio(const Float& valor) const
	{
		return atan(mConstanteEquilibrio * valor) * 2.0 / PI;
	}

	template class Algoritmo<RNL>;
}