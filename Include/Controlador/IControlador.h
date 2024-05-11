#ifndef _ICONTROLADOR_H_
#define _ICONTROLADOR_H_

// Incluir Utils
#include "Tipos.h"

class IControlador
{
	public:
		virtual Vetor4D Calcular() = 0;
		virtual Void Posicao(const Vetor3D& posicao) = 0;
		virtual Void Rotacao(const Vetor3D& rotacao) = 0;
		virtual Void VelocidadeAngular(const Vetor3D& velocidade) = 0;
		virtual Void VelocidadeLinear(const Vetor3D& velocidade) = 0;
};

#endif //_ICONTROLADOR_H_