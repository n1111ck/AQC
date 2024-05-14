#ifndef _GERENCIADOR_ACOPLADORES_H_
#define _GERENCIADOR_ACOPLADORES_H_

// Incluir Acoplador
#include "Acoplador.h"

// Incluir Modelo
#include "Modelo.h"

// Incluir Utils
#include "Tipos.h"

namespace AQC
{
	class GerenciadorAcopladores
	{
	public:
		GerenciadorAcopladores(
			const Float& limiteTensao,
			const Float& relacaoVelocidade,
			const Float& relacaoForca,
			const Float& relacaoTorque,
			const Float& raio
		);
		~GerenciadorAcopladores();

		Void Simulacao(Modelo& modelo);

		Float SomatorioRotacao() const;
		Void Aplicar(const Vetor4D& entrada);
	private:
		Acoplador mpAcoplador[4];
		Modelo* mpModelo;

		Float mRelacaoVelocidade;
		Vetor4D mUltimaVelocidade;
		Float mLimiteTensao;
	};
}

#endif // _GERENCIADOR_ACOPLADORES_H_