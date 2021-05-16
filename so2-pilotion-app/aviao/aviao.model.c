#pragma once

#include <process.h>
#include "../control/main.helper.h"
#include "aviao.model.h"

/**
 * @inheritDoc
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR dadosAeroporto[2][BUFFER_SIZE])
{
	Aviao Aviao;

	Aviao.PID = PID == -1 ? GetCurrentProcessId() : PID;
	Aviao.maxPassag = maxPassag;
	Aviao.coordenadasPorSegundo = coordenadasPorSegundo;

	memset(Aviao.siglaAeroportoPartida, 0, sizeof(Aviao.siglaAeroportoPartida));
	wcscpy_s(Aviao.siglaAeroportoPartida, 200, dadosAeroporto[0]);
	memset(Aviao.siglaAeroportoDestino, 0, sizeof(Aviao.siglaAeroportoDestino));
	wcscpy_s(Aviao.siglaAeroportoDestino, 200, dadosAeroporto[1]);

	return Aviao;
}


int moveAviao(int cur_x, int cur_y, int final_x, int final_y, int* next_x, int* next_y) 
{
	int *proxCoord[] = { 0,0 };
	int movimento = move(cur_x, cur_y, final_x, final_y, &proxCoord[0], &proxCoord[1]);
	if (movimento == 0)
		// procurar alternativas de deslocação (duas alternativas)
		return 0;
	else {
		return 1;
	}
	// ver o que retorna 
}

bool aviaoViaja(int partida_x, int partida_y, int destino_x, int destino_y)
{	
	int teste = moveAviao(partida_x, partida_y, destino_x, destino_y, 0, 0);
	return teste;
}


/**
 * @inheritDoc
**/
//Passageiro *embarcarPassageiro(Passageiro *passageiro, Aviao *aviao)
//{
//	int capacidade = sizeof(aviao->passageiros) / sizeof(Passageiro);
//	if (aviao->maxPassag == capacidade)
//	{
//		// enviar mensagem para avisar que a maxima capacidade foi atingida
//	}
//
//	if (passageiro->siglaAeroportoDestino == aviao->siglaAeroportoPartida)
//		aviao->passageiros[capacidade - 1] = passageiro;
//
//	return passageiro;
//}
