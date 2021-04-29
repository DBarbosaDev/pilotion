#pragma once

#include <windows.h>
#include <tchar.h>
#include "passageiro.model.h"

typedef struct Aviao {
	DWORD PID;
	int maxPassag;

	int coordenadasPorSegundo;
	
	TCHAR* nomeAeroportoPartida;
	TCHAR* siglaAeroportoPartida;
	TCHAR* nomeAeroportoDestino;
	TCHAR* siglaAeroportoDestino;

	Passageiro passageiros[];

} Aviao;

/**
 *	Ordem obrigatória de envio de dados no método novoAviao
 *	dadosAeroporto[0] = sigla aeroporto de partida
 *  dadosAeroporto[1] = sigla do aeroporto de destino
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR* dadosAeroporto[]);

/**
 *	Embarcar um passageiro no respetivo aviao
 *	caso o aviao ainda não tenha atigido a sua capacidade máxima 
 *  e o destino de ambos seja o mesmo, o passageiro pode embarcar
**/
Passageiro embarcarPassageiro(Passageiro* passageiro, Aviao* aviao);