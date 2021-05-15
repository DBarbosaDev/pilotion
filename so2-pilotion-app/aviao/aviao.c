// aviao.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "aviao.model.h"
#include "../control/constants.h"

int _tmain()
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
    
    _tprintf(_TEXT("novoaviao;\n\n"));
    _tprintf(_TEXT("Numero maximo de passageiros;\n"));
    wscanf_s(_T("%d"), &maxPassag);

    _tprintf(_TEXT("Coordenadas por segundo;\n"));
    wscanf_s(_T("%d"), &coordenadasPorSegundo);

    _tprintf(_TEXT("siglaAeroportoPartida;\n"));
    wscanf_s(_T("%199s"), &dados[0], 200);

    _tprintf(_TEXT("siglaAeroportoDestino;\n"));
    wscanf_s(_T("%199s"), &dados[1], 200);

    Aviao nAviao = novoAviao(-1, maxPassag, coordenadasPorSegundo, dados);
    wprintf(_T("%d"), nAviao.maxPassag);
    wprintf(_T("%s"), nAviao.siglaAeroportoDestino);
    wprintf(_T("%s"), nAviao.siglaAeroportoPartida);

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

    pBuf = (Aviao*) MapViewOfFile(hMapFile, // handle to map object
        FILE_MAP_ALL_ACCESS,  // read/write permission
        0,
        0,
        sizeof(Aviao) * 10);

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }

    Aviao* pnAviao = &nAviao;

    CopyMemory(pBuf, pnAviao, sizeof(Aviao));

    //CopyMemory(pBuf, nAviao, 10)

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    system("pause");
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
