// aviao.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "aviao.model.h"
#include "communication.h"
#include "ui.console.h"

#include "../control/constants.h"
#include "../control/main.helper.h"

int _tmain()
{
    #ifdef UNICODE
        _setmode(_fileno(stdin), _O_WTEXT);
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stderr), _O_WTEXT);
    #endif

    HANDLE hMapFile;
    TCHAR dados[2][200];
    for (size_t i = 0; i < 2; i++)
        memset(dados[i], 0, 200);
    int maxPassag = 0;
    int coordenadasPorSegundo = 0;
    
    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        SHARED_MEMORY_STACK_PLANES);

    if (hMapFile == NULL) {
        wprintf(TEXT("\n>> O controlador não está a correr. Tente mais tarde\n"));
        return 1;
    }

    iniciaUI(&maxPassag, &coordenadasPorSegundo, &dados);

    Aviao nAviao = novoAviao(-1, maxPassag, coordenadasPorSegundo, dados);

    WaitForSingleObject(nAviao.Threads.hConfirmacaoConexao, INFINITE);

    adicionaAviaoToStack(&nAviao, hMapFile);

    aviaoViaja(0, 0, 10, 10);

    system("pause");

    CloseHandle(hMapFile);
    return 0;
}