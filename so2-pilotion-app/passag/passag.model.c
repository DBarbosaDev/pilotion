#pragma once
#include <strsafe.h>
#include "passag.model.h"

/**
 * @inheritDoc
**/
Passageiro novoPassageiro(int id, int tempoEspera, TCHAR nome[100],TCHAR dados[2][200])
{
	Passageiro passag;

	passag.id = id;
	passag.tempoEspera = tempoEspera;

	memset(passag.siglaAeroportoPartida, 0, sizeof(passag.siglaAeroportoPartida));
	wcscpy_s(passag.siglaAeroportoPartida, 200, dados[0]);
	memset(passag.siglaAeroportoDestino, 0, sizeof(passag.siglaAeroportoDestino));
	wcscpy_s(passag.siglaAeroportoDestino, 200, dados[1]);
	memset(passag.nome, 0, sizeof(passag.nome));
	wcscpy_s(passag.nome, 100, nome);

	//passag.nome = nome;
	//passag.siglaAeroportoPartida = dados[0];
	//passag.siglaAeroportoDestino = dados[1];

	return passag;
}

