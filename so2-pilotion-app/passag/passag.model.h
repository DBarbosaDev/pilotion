#pragma once

#include <windows.h>
#include <tchar.h>

typedef struct Passageiro {
	int id;
	int tempoEspera;
	TCHAR nome[100];
	TCHAR siglaAeroportoPartida[200];
	TCHAR siglaAeroportoDestino[200];
	BOOL embarcado;
} Passageiro;

/**
 *	Ordem obrigat�ria de envio de dados no m�todo novoPassageiro
 *	args[0] = nome do passageiro
 *  args[1] = sigla do aeroporto de partida
 *	args[2] = sigla do aeroporto de destino
**/
Passageiro novoPassageiro(int id, int tempoEspera, TCHAR nome[100], TCHAR dados[2][200]);