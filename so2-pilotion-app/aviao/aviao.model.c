#pragma once

#include <process.h>
#include <strsafe.h>
#include "threads_routines.h"
#include "aviao.model.h"
#include "../control/constants.h"
#include "../control/main.helper.h"
#include "../control/communication.model.h"


/**
 * @inheritDoc
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR dadosAeroporto[2][BUFFER_SIZE])
{
	Aviao aviao;

	aviao.PID = PID == -1 ? GetCurrentProcessId() : PID;
	aviao.maxPassag = maxPassag;
	aviao.coordenadasPorSegundo = coordenadasPorSegundo;
	aviao.coordenadasAtuais[0] = 0;
	aviao.coordenadasAtuais[1] = 0;
	aviao.nrMaximoDeAvioes = getIntValueFromSharedMemory(SHARED_MEMORY_STACK_LENGTH_VALUE);

	memset(aviao.siglaAeroportoPartida, 0, sizeof(aviao.siglaAeroportoPartida));
	wcscpy_s(aviao.siglaAeroportoPartida, BUFFER_SIZE, dadosAeroporto[0]);
	memset(aviao.siglaAeroportoDestino, 0, sizeof(aviao.siglaAeroportoDestino));
	wcscpy_s(aviao.siglaAeroportoDestino, BUFFER_SIZE, dadosAeroporto[1]);

	aviao.estadoConexaoComControl = CONNECTION_REQUEST;

	memset(aviao.eventos.nomeEventoAeroportosInvalidos, 0, sizeof(aviao.eventos.nomeEventoAeroportosInvalidos));
	memset(aviao.eventos.nomeEventoConfirmacaoConexao, 0, sizeof(aviao.eventos.nomeEventoConfirmacaoConexao));
	memset(aviao.eventos.nomeEventoParaSair, 0, sizeof(aviao.eventos.nomeEventoParaSair));

	iniciarEventos(&aviao);
	iniciarThreads(&aviao);

	return aviao;
}

void iniciarEventos(Aviao* aviao) {
	TCHAR* stringPID = malloc(sizeof(TCHAR) * 10);
	_itow_s(aviao->PID, stringPID, sizeof(TCHAR) * 10, 10);
	
	wcscpy_s(aviao->eventos.nomeEventoAeroportosInvalidos, BUFFER_SIZE, _T("Global\\nomeEventoAeroportosInvalidos"));
	StringCchCat(aviao->eventos.nomeEventoAeroportosInvalidos, BUFFER_SIZE, stringPID);

	wcscpy_s(aviao->eventos.nomeEventoConfirmacaoConexao, BUFFER_SIZE, _T("Global\\nomeEventoConfirmacaoConexao"));
	StringCchCat(aviao->eventos.nomeEventoConfirmacaoConexao, BUFFER_SIZE, stringPID);
	
	wcscpy_s(aviao->eventos.nomeEventoParaSair, BUFFER_SIZE, _T("Global\\nomeEventoParaSair"));
	StringCchCat(aviao->eventos.nomeEventoParaSair, BUFFER_SIZE, stringPID);
	
	memset(stringPID, 0, sizeof(stringPID));

	aviao->eventos.handleEventoConfirmacaoConexao = CreateEvent(NULL, TRUE, FALSE, aviao->eventos.nomeEventoConfirmacaoConexao);
	aviao->eventos.handleEventoAeroportosInvalidos = CreateEvent(NULL, TRUE, FALSE, aviao->eventos.nomeEventoAeroportosInvalidos);
	aviao->eventos.handleEventoParaSair = CreateEvent(NULL, TRUE, FALSE, aviao->eventos.nomeEventoParaSair);
}

void iniciarThreads(Aviao* aviao) {
	aviao->Threads.hAeroportosInvalidos = CreateThread(NULL, 0, routinaAeroportosInvalidos, aviao, 0, NULL);
	aviao->Threads.hConfirmacaoConexao = CreateThread(NULL, 0, routinaConfirmacaoConexao, aviao, 0, NULL);
	aviao->Threads.hControloDeSaida = CreateThread(NULL, 0, routinaControloDeSaida, aviao, 0, NULL);
}

int moveAviao(int cur_x, int cur_y, int final_x, int final_y, int* next_x, int* next_y) 
{
	HANDLE hMapFile;
	int proxCoord[] = { cur_x, cur_y };

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		SHARED_MEMORY_STACK_PLANES);               // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
		return 1;
	}

	Aviao* listaAvioes = getPlanesStackPointer(hMapFile, 10);

	while (proxCoord[0] != final_x && proxCoord[1] != final_y)
	{
		for (size_t i = 0; i < 10; i++)
		{
			Aviao* elem = (Aviao*)&listaAvioes[i];

			if (elem->PID == GetCurrentProcessId()) {
				elem->coordenadasAtuais[0] = proxCoord[0];
				elem->coordenadasAtuais[1] = proxCoord[1];
			}

			if (elem->coordenadasAtuais[0] == 0 && next_x == 0)
				continue;

			if (elem->coordenadasAtuais[0] == next_x && elem->coordenadasAtuais[1] == next_y) {
				proxCoord[0]++;
			}
		}
		int movimento = move(proxCoord[0], proxCoord[1], final_x, final_y, &proxCoord[0], &proxCoord[1]);
		_tprintf(_TEXT("\ntravelling to (%d, %d)"), proxCoord[0], proxCoord[1]);
		Sleep(1000);
	}

	_tprintf(_TEXT("O avião chegou ao destino nas coordenadas (%d, %d)"), proxCoord[0], proxCoord[1]);
	return 1;
}

BOOL aviaoViaja(int partida_x, int partida_y, int destino_x, int destino_y)
{	
	int teste = moveAviao(partida_x, partida_y, destino_x, destino_y, partida_x, partida_y);
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
