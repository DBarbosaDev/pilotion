#include "passag.console.h"

void iniciaUI(TCHAR nome[100], TCHAR dadosAeroporto[2][200], int* tempoEspera)
{
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||                       PILOTION                   ||\n"));
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||                    Nome do passageiro            ||\n"));
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||  -> (string) : "));
	wscanf_s(_T("%99s"), &nome, 100);
	_tprintf(_TEXT("||                  Tempo de espera                 ||\n"));
	_tprintf(_TEXT("||  -> (numero inteiro) : "));
	wscanf_s(_T("%d"), tempoEspera, sizeof(int));
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||           Aeroporto de Partida (SIGLA)           ||\n"));
	_tprintf(_TEXT("||  -> (string com 3 caracteres) : "));
	wscanf_s(_T("%199s"), &dadosAeroporto[0], 200);
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||           Aeroporto de Destino (SIGLA)           ||\n"));
	_tprintf(_TEXT("||  -> (string com 3 caracteres) : "));
	wscanf_s(_T("%199s"), &dadosAeroporto[1], 200);
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||                A construir passageiro            ||\n"));
	_tprintf(_TEXT("======================================================\n"));
}