// passag.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "passag.model.h"


int _tmain(int argc, char* argv[])
{
#ifdef UNICODE
    _setmode(_fileno(stdin), _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stderr), _O_WTEXT);
#endif

    Passageiro* passag = NULL;
    TCHAR dados[2][200];
    for (size_t i = 0; i < 2; i++)
        memset(dados[i], 0, 200);

    TCHAR nome[200];
    memset(nome, 0, 200);
    int *tempoEspera = 0;

    iniciaUI(nome, dados, tempoEspera);
    Passageiro nPassag = novoPassageiro(-1, nome, dados[0], dados[1], tempoEspera);
    
    wprintf(_T("Olá Mundo de passageiros"));
}