#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "aviao.model.h"
#include "communication.h"
#include "ui.console.h"

#include "../control/constants.h"
#include "../control/main.helper.h"

int WINAPI ConsoleHandler(DWORD CEvent) {
    HANDLE hSemaforo = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SHARED_MEMORY_STACK_SEMAPHORE);

    switch (CEvent) {
    case CTRL_CLOSE_EVENT:
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
        ReleaseSemaphore(hSemaforo, 1, NULL);
        CloseHandle(hSemaforo);
        exit(0);
        break;
    case CTRL_C_EVENT:
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
        ReleaseSemaphore(hSemaforo, 1, NULL);
        CloseHandle(hSemaforo);
        exit(0);
        break;
    }
}

int _tmain(int argc, char* argv[])
{
    #ifdef UNICODE
        _setmode(_fileno(stdin), _O_WTEXT);
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stderr), _O_WTEXT);
    #endif

    Aviao* pAviaoStack = NULL;
    HANDLE hMapFile, hSemaforo;
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
        wprintf(TEXT("\n>> O controlador n�o est� a correr. Tente mais tarde\n"));
        system("pause");
        return 1;
    }

    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {
        _wperror(_T("ERROR: O programa n�o consegui carregar o handler da consola.\n"));
        exit(1);
    }

    hSemaforo = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SHARED_MEMORY_STACK_SEMAPHORE);
    iniciaUI(&maxPassag, &coordenadasPorSegundo, &dados);

    wprintf(TEXT("\n>> A aguardar por disponibilidade...\n"));
    DWORD waitForStackLength = WaitForSingleObject(hSemaforo, INFINITE);

    switch (waitForStackLength)
    {
        case WAIT_OBJECT_0: 
            Aviao nAviao = novoAviao(-1, maxPassag, coordenadasPorSegundo, dados);
            pAviaoStack = adicionaAviaoToStack(&nAviao, hMapFile);
            wprintf(_T("\n>> O Controlador aceitou a conexão com sucesso.\n"));

            if (!ReleaseSemaphore(hSemaforo, 1, NULL))
                wprintf(_T("Release Semaphore error\n"));
            break;
        case WAIT_TIMEOUT:
            _tprintf(TEXT("Could not receive any sign of num items change. %d\n"), GetLastError());
            break;
        default:
            break;
    }

    CloseHandle(hSemaforo);
    CloseHandle(hMapFile);

    while(1){
        TCHAR command[200];
        _tprintf(_TEXT("$>"));
        wscanf_s(_T("%199s"), &command, 200);
    }

    return 0;
}