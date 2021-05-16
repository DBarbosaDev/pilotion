#include <tchar.h>

void iniciaUI(int *maxPassag, int *coordenadasPorSegundo, TCHAR* dadosAeroporto[]) 
{	
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||                       PILOTION                   ||\n"));
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||            Capacidade maxima do aviao            ||\n"));
	_tprintf(_TEXT("||  -> (numero inteiro) : "));
	wscanf_s(_T("%d\n"), &maxPassag);
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||              Velocidade por segundo              ||\n"));
	_tprintf(_TEXT("||  -> (numero inteiro) : "));
	wscanf_s(_T("%d\n"), &coordenadasPorSegundo);
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||           Aeroporto de Partida (SIGLA)           ||\n"));
	_tprintf(_TEXT("||  -> (string com 3 caracteres) : "));
	wscanf_s(_T("%199s\n"), &dadosAeroporto[0], 200);
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||           Aeroporto de Destino (SIGLA)           ||\n"));
	_tprintf(_TEXT("||  -> (string com 3 caracteres) : "));
	wscanf_s(_T("%199s\n"), &dadosAeroporto[1], 200);
	_tprintf(_TEXT("======================================================\n"));
	_tprintf(_TEXT("||                A construir Aviao                 ||\n"));
	_tprintf(_TEXT("======================================================\n"));

}