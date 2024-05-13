#include "Algoritmo.h"
#include <math.h>

// Incluir Controlador
#include "RNL.h"

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
	mArfagemAvanco(5 * atan(1) / 45.0),
	mToleranciaEntrega(10.0),
	mToleranciaPouso(2.0),
	mLatitudeDestino(0.0),
	mLongitudeDestino(0.0)
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
	// Tarefa executada
	// Inoperante

	// Condicao de transicao
	if (mIniciar)
	{
		mIniciar = false;
		mEstadoAtual = eDecolagem;
		mLatitudeBase = mpGerenciadorSensores->Posicao().mX;
		mLongitudeBase = mpGerenciadorSensores->Posicao().mY;
	}
}

template<typename Controlador>
Void
Algoritmo<Controlador>::TarefaDecolagem()
{
	// Tarefa executada
	mpControlador->Aplicar({
		mAltitudeVoo,
		0.0,
		0.0,
		atan2f(
			mLongitudeDestino - mpGerenciadorSensores->Posicao().mX,
			mLatitudeDestino - mpGerenciadorSensores->Posicao().mY
		)
	});


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
	// TODO: fazer a correcao por rolamento

	// Tarefa executada
	mpControlador->Aplicar({
		mAltitudeVoo,
		0.0,
		mArfagemAvanco,
		atan2f(
			mLongitudeDestino - mpGerenciadorSensores->Posicao().mX,
			mLatitudeDestino - mpGerenciadorSensores->Posicao().mY
		)
	});

	// Condicao de transicao
	if (pow(pow(mLongitudeDestino - mpGerenciadorSensores->Posicao().mX, 2) +
		pow(mLatitudeDestino - mpGerenciadorSensores->Posicao().mY, 2), 0.5) < mToleranciaEntrega)
	{
		mEstadoAtual = ePouso;
	}
}

template<typename Controlador>
Void
Algoritmo<Controlador>::TarefaPouso()
{
	// Tarefa executada
	mpControlador->Aplicar({
		0.0,
		0.0,
		0.0,
		mpGerenciadorSensores->Rotacao().mZ
	});

	// Condicao de transicao
	if (!(mpGerenciadorSensores->Baixo() > mToleranciaPouso))
	{
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
	mpControlador->Aplicar({
		mpGerenciadorSensores->Posicao().mZ,
		0.0,
		0.0,
		mpGerenciadorSensores->Rotacao().mZ
	});
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
	mpControlador->Aplicar({
		mpGerenciadorSensores->Posicao().mZ,
		0.0,
		0.0,
		mpGerenciadorSensores->Rotacao().mZ
	});
	Resetar();

	// Fim de Operacao
}

template<typename Controlador>
Void
Algoritmo<Controlador>::TarefaPrevencaoColisao()
{
	// TODO: pensar num algoritmo
}

template<typename Controlador>
Void
Algoritmo<Controlador>::Atualizar()
{
	MaquinaEstados();
}

template class Algoritmo<RNL>;