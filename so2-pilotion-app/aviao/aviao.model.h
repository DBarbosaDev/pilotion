#pragma once

#include <windows.h>
#include <tchar.h>
#include "passageiro.model.h"
#include "./so2dll/SO2_TP_DLL_2021.h"


#define MAXPASSAG 10

typedef struct Aviao
{
	DWORD PID;
	int maxPassag;

	int coordenadasPorSegundo;
	TCHAR siglaAeroportoPartida[200];
	TCHAR siglaAeroportoDestino[200];

	//Passageiro *passageiros[MAXPASSAG];
	int passageiros;

} Aviao;

/**
 *	Ordem obrigat�ria de envio de dados no m�todo novoAviao
 *	dadosAeroporto[0] = sigla aeroporto de partida
 *  dadosAeroporto[1] = sigla do aeroporto de destino
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR *dadosAeroporto[]);

/**
 *	Embarcar um passageiro no respetivo aviao
 *	caso o aviao ainda n�o tenha atigido a sua capacidade m�xima
 *  e o destino de ambos seja o mesmo, o passageiro pode embarcar
**/
Passageiro *embarcarPassageiro(Passageiro *passageiro, Aviao *aviao);
