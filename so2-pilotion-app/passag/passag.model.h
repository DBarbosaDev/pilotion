#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdbool.h>

typedef struct Passageiro{
	int pid;
	int tempoEspera;
	TCHAR* nome;
	TCHAR* siglaAeroportoPartida;
	TCHAR* siglaAeroportoDestino;
	bool embarcado;
} Passageiro;

Passageiro novoPassageiro(DWORD PID, TCHAR* nome, TCHAR* siglaAeroportoPartida, TCHAR* siglaAeroportoDestino, int tempoEspera);