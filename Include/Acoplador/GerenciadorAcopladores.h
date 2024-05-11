#ifndef _GERENCIADOR_ACOPLADORES_H_
#define _GERENCIADOR_ACOPLADORES_H_

// Incluir Acoplador
#include "Acoplador.h"

// Incluir Modelo
#include "Modelo.h"

// Incluir Utils
#include "Tipos.h"

class GerenciadorAcopladores
{
	public:
		GerenciadorAcopladores(
			const Float& relacaoVelocidade,
			const Float& relacaoForca,
			const Float& relacaoTorque,
			const Float& constanteTempo,
			const Float& raio
		);
		~GerenciadorAcopladores();

		Void Simulacao(Modelo& modelo);

		Void Aplicar(const Vetor4D& entrada) const;
	private:
		Acoplador mpAcoplador[4];
		Modelo* mpModelo;

		Float mRelacaoVelocidade;
};

#endif // _GERENCIADOR_ACOPLADORES_H_