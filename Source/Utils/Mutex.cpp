#include "Mutex.h"

namespace AQC
{
	Mutex::Mutex() :
		mEstado(Estado::DESTRAVADO)
	{

	}

	Mutex::~Mutex()
	{

	}

	Void
	Mutex::Destravar()
	{
		mEstado = Estado::DESTRAVADO;
	}

	Void
	Mutex::Travar()
	{
		while (mEstado == Estado::TRAVADO)
		{
			// Vazio
		}

		mEstado = Estado::TRAVADO;
	}

	Boolean
	Mutex::TentarTravar()
	{
		Boolean resultado = false;

		if (mEstado == Estado::DESTRAVADO)
		{
			mEstado = Estado::TRAVADO;
			resultado = true;
		}

		return resultado;
	}
}