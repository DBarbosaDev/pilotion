#pragma once

#include <windows.h>
#include <tchar.h>
#include "passageiro.model.h"
#include "SO2_TP_DLL_2021\SO2_TP_DLL_2021.h"

#define MAXPASSAG 10
#define BUFFER_SIZE 200

typedef struct Eventos {
	HANDLE handleEventoConfirmacaoConexao;
	TCHAR nomeEventoConfirmacaoConexao[BUFFER_SIZE];

	HANDLE handleEventoAeroportosInvalidos;
	TCHAR nomeEventoAeroportosInvalidos[BUFFER_SIZE];

	HANDLE handleEventoParaSair;
	TCHAR nomeEventoParaSair[BUFFER_SIZE];
} Eventos;

typedef struct Threads {
	HANDLE hConfirmacaoConexao;
	HANDLE hAeroportosInvalidos;
	HANDLE hControloDeSaida;
} Threads;

typedef struct Aviao
{
	DWORD PID;

	int maxPassag;
	int coordenadasPorSegundo;
	// x = coordenadasAtuais[0] || y = coordenadasAtuais[1]
	int coordenadasAtuais[2];
	//Passageiro *passageiros[MAXPASSAG];
	int passageiros;

	TCHAR siglaAeroportoPartida[200];
	TCHAR siglaAeroportoDestino[200];
	
	int estadoConexaoComControl;

	Eventos eventos;
	Threads Threads;
} Aviao;

/**
 *	Ordem obrigat�ria de envio de dados no m�todo novoAviao
 *	dadosAeroporto[0] = sigla aeroporto de partida
 *  dadosAeroporto[1] = sigla do aeroporto de destino
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR dadosAeroporto[2][BUFFER_SIZE]);

void iniciarEventos(Aviao* aviao);

void iniciarThreads(Aviao* aviao);

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