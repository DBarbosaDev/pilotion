#pragma once

#include "passag.model.h"

/**
 * @inheritDoc
**/
Passageiro novoPassageiro(int id, int tempoEspera, TCHAR* args[])
{
	Passageiro passag;

	passag.id = id;
	passag.tempoEspera = tempoEspera;
	passag.nome = args[0];
	passag.siglaAeroportoPartida = args[1];
	passag.siglaAeroportoDestino = args[2];

	return passag;
}

