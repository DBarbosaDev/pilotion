#pragma once

#include <windows.h>
#include <tchar.h>
#include "passageiro.model.h"
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
	int coordenadasAtuais[2]; // x = coordenadasAtuais[0] || y = coordenadasAtuais[1]
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