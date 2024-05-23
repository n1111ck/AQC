#ifndef _GERENCIADOR_ACOPLADORES_H_
#define _GERENCIADOR_ACOPLADORES_H_

// Incluir Acoplador
#include "Acoplador/Acoplador.h"

// Incluir Modelo
#include "Modelo/Modelo.h"

// Incluir Utils
#include "Utils/Tipos.h"

namespace AQC
{
	class GerenciadorAcopladores
	{
	public:
		GerenciadorAcopladores();
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
		Vetor4D Tensao() const;
		Void Aplicar(const Vetor4D& entrada);
	private:
		Acoplador mpAcoplador[4];
		Modelo* mpModelo;

		Float mRelacaoVelocidade;
		Vetor4D mUltimaVelocidade;
		Vetor4D mUltimaTensao;
		Float mLimiteTensao;
	};
}

#endif // _GERENCIADOR_ACOPLADORES_H_