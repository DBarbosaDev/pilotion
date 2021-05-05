// aviao.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "aviao.model.h"

int _tmain()
{
#ifdef UNICODE
    _setmode(_fileno(stdin), _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stderr), _O_WTEXT);
#endif

    int maxPassag = 0;
    TCHAR coordenadasPorSegundo = "\0";
    TCHAR dados[2];

    _tprintf(_TEXT("novoaviao;\n\n"));
    _tprintf(_TEXT("Numero maximo de passageiros;\n"));
    wscanf_s(_T("%d"), &maxPassag, 200);

    _tprintf(_TEXT("Coordenadas por segundo;\n"));
    wscanf_s(_T("%4s"), &coordenadasPorSegundo, 200);

    _tprintf(_TEXT("siglaAeroportoPartida;\n"));
    wscanf_s(_T("%lc"), &dados[0], 200);

    _tprintf(_TEXT("siglaAeroportoDestino;\n"));
    wscanf_s(_T("%lc"), &dados[1], 200);

    Aviao nAviao = novoAviao(0, maxPassag, coordenadasPorSegundo, dados);
    printf("%d", nAviao.maxPassag);
    printf("%lc", nAviao.siglaAeroportoDestino);
    printf("%lc", nAviao.siglaAeroportoPartida);

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
