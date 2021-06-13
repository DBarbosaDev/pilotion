#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "aviao.model.h"
#include "communication.h"
#include "ui.console.h"

#include "../control-gui/constants.h"
#include "../control-gui/communication.model.h"
#include "../control-gui/main.helper.h"

int WINAPI ConsoleHandler(DWORD CEvent) {
    HANDLE semaforoControloLotacao = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SHARED_MEMORY_STACK_SEMAPHORE);

    switch (CEvent) {
    case CTRL_CLOSE_EVENT:
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
        ReleaseSemaphore(semaforoControloLotacao, 1, NULL);
        CloseHandle(semaforoControloLotacao);
        exit(0);
        break;
    case CTRL_C_EVENT:
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
        ReleaseSemaphore(semaforoControloLotacao, 1, NULL);
        CloseHandle(semaforoControloLotacao);
        exit(0);
        break;
    }
}

void adicionarItemNaStack(Aviao* aviao) {
    aviao->pAviaoStack = adicionaAviaoToStack(aviao, aviao->HandlesAplicacao.hMapFile);
    ReleaseSemaphore(aviao->HandlesAplicacao.hSemaforoParaLeituraItem, 1, NULL);
}

int _tmain(int argc, char* argv[])
{
    #ifdef UNICODE
        _setmode(_fileno(stdin), _O_WTEXT);
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stderr), _O_WTEXT);
    #endif

    Aviao aviao;

    TCHAR dados[2][200];
    for (size_t i = 0; i < 2; i++)
        memset(dados[i], 0, 200);
    int maxPassag = 0;
    int coordenadasPorSegundo = 0;
    

    aviao.HandlesAplicacao.hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        SHARED_MEMORY_STACK_PLANES);

    if (aviao.HandlesAplicacao.hMapFile == NULL) {
        wprintf(TEXT("\n>> O controlador n�o est� a correr. Tente mais tarde\n"));
        system("pause");
        return 1;
    }

    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {
        _wperror(_T("ERROR: O programa n�o consegui carregar o handler da consola.\n"));
        exit(1);
    }

    wprintf(TEXT("\n>> A aguardar por disponibilidade...\n"));

    aviao.HandlesAplicacao.hSemaforoControloLotacao = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SHARED_MEMORY_STACK_SEMAPHORE);
    aviao.HandlesAplicacao.hSemaforoParaLeituraItem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SHARED_MEMORY_STACK_SEMAPHORE_NUM_ITEM);
    
    switch (WaitForSingleObject(aviao.HandlesAplicacao.hSemaforoControloLotacao, INFINITE))
    {
        case WAIT_OBJECT_0:

            iniciaUI(&maxPassag, &coordenadasPorSegundo, &dados);
            aviao = novoAviao(aviao, -1, maxPassag, coordenadasPorSegundo, dados);
            adicionarItemNaStack(&aviao);

            iniciarThreads(&aviao);

            break;
        default:
            break;
    }

    WaitForSingleObject(aviao.Threads.hAeroportosInvalidos, INFINITE);

    CloseHandle(aviao.eventos.handleEventoAeroportosInvalidos);
    CloseHandle(aviao.eventos.handleEventoConfirmacaoConexao);

    system("cls");

    wprintf(_T("\n>> Conexão aceite com sucesso.\n"), GetCurrentThreadId());

    while(1){
        fflush(stdout);
        fflush(stdin);
        TCHAR command[200];
        _tprintf(_TEXT("$>"));
        wscanf_s(_T("%199s"), &command, 200);
    }

    ReleaseSemaphore(aviao.HandlesAplicacao.hSemaforoControloLotacao, 1, NULL);
    
    CloseHandle(aviao.HandlesAplicacao.hSemaforoControloLotacao);
    CloseHandle(aviao.HandlesAplicacao.hMapFile);

    return 0;
}