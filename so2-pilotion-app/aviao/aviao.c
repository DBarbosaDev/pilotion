// aviao.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "aviao.model.h"
#include "../control/constants.h"
#include "communication.h"
#include "ui.console.h"

int _tmain(int argc, char* argv[])
{
#ifdef UNICODE
    _setmode(_fileno(stdin), _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stderr), _O_WTEXT);
#endif

    int maxPassag = 0;
    int coordenadasPorSegundo = 0;
    TCHAR dados[2][200];
    for (size_t i = 0; i < 2; i++)
        memset(dados[i], 0, 200);

    // trocar por argumentos de consola
    iniciaUI(&maxPassag, &coordenadasPorSegundo, &dados);

    Aviao nAviao = novoAviao(-1, maxPassag, coordenadasPorSegundo, dados);

    WaitForSingleObject(nAviao.Threads.hConfirmacaoConexao, INFINITE);

    HANDLE hMapFile;
    Aviao *pBuf;

    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        REGISTRY_SHARED_MEMORY_STACK_PLANES_VALUE);               // name of mapping object

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }

    pBuf = adicionaAviaoToStack(&nAviao, hMapFile);

    CloseHandle(hMapFile);

    aviaoViaja(0, 0, 10, 10);

    system("pause");
    return 0;
}