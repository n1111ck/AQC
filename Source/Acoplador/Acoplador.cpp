#include "Acoplador/Acoplador.h"

namespace AQC
{
	Acoplador::Acoplador()
	{
		sMotorCounter++;
	}

	Acoplador::~Acoplador()
	{

	}

	Void
	Acoplador::Resetar()
	{
		sMotorCounter = 0;
	}

	UInt8 Acoplador::sMotorCounter = 0;
}