#pragma once

#include <tchar.h>
#include <stdbool.h>

/*typedef struct Passageiro {
	int id;
	int tempoEspera;
	TCHAR* nome;
	TCHAR* siglaAeroportoPartida;
	TCHAR* siglaAeroportoDestino;
	bool embarcado;
} Passageiro;*/

/**
 *	Ordem obrigatória de envio de dados no método novoPassageiro
 *	args[0] = nome do passageiro
 *  args[1] = sigla do aeroporto de partida
 *	args[2] = sigla do aeroporto de destino
**/
//Passageiro novoPassageiro(int id, int tempoEspera, TCHAR* args[]);