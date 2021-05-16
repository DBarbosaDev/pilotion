#pragma once

#include <windows.h>
#include <tchar.h>
#include <process.h>
#include "passageiro.model.h"
#include "../control/main.helper.h"
#include "SO2_TP_DLL_2021\SO2_TP_DLL_2021.h"

#define MAXPASSAG 10
#define BUFFER_SIZE 200

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
 *	Ordem obrigatoria de envio de dados no metodo novoAviao
 *	dadosAeroporto[0] = sigla aeroporto de partida
 *  dadosAeroporto[1] = sigla do aeroporto de destino
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR dadosAeroporto[2][BUFFER_SIZE]);

/**
 *	Embarcar um passageiro no respetivo aviao
 *	caso o aviao ainda n�o tenha atigido a sua capacidade m�xima
 *  e o destino de ambos seja o mesmo, o passageiro pode embarcar
**/
Passageiro *embarcarPassageiro(Passageiro *passageiro, Aviao *aviao);

/**
 *	Embarcar um passageiro no respetivo aviao
 *	caso o aviao ainda n�o tenha atigido a sua capacidade m�xima
 *  e o destino de ambos seja o mesmo, o passageiro pode embarcar
**/
int moveAviao(int cur_x, int cur_y, int final_x, int final_y, int* next_x, int* next_y);

/**
 *	Simular a viagem do aviao de aeroporto em aeroporto
**/
bool aviaoViaja(int partida_x, int partida_y, int destino_x, int destino_y);