#include "AQC.h"

// Incluir Controlador
#include "RNL.h"

template<typename Controlador>
AQC<Controlador>::AQC(
	Controlador& controlador,
	GerenciadorAcopladores& gerenciadorAcopladores,
	GerenciadorSensores& gerenciadorSensores
):
	mpControlador(&controlador),
	mpGerenciadorSensores(&gerenciadorSensores),
	mpGerenciadorAcopladores(&gerenciadorAcopladores)
{

}

template<typename Controlador>
AQC<Controlador>::~AQC()
{

}

template class AQC<RNL>;