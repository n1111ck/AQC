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

		Acoplador(
			const Float& relacaoVelocidade,
			const Float& relacaoForca,
			const Float& relacaoTorque,
			const Float& raio
		);
		~Acoplador();

		Float Calcular(const Vetor4D& entrada) const;
	private:
		Float mRelacaoVelocidade;
		Float mRelacaoForca;
		Float mRelacaoTorque;
		Float mRaio;

		Linha4D mLinhaConversao;

		static UInt8 sMotorCounter;
	};
}

#endif // _ACOPLADOR_H_