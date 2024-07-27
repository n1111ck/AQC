#ifndef _ACOPLADOR_H_
#define _ACOPLADOR_H_

// Incluir Utils
#include "Utils/Tipos.h"
namespace AQC
{
	class Acoplador
	{
	public:
		Acoplador();
		~Acoplador();

		static Void Resetar();
	private:
		static UInt8 sMotorCounter;
	};
}

#endif // _ACOPLADOR_H_