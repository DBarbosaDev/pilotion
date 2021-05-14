#include <stdio.h>
#include <tchar.h>
#include <fcntl.h>
#include <windows.h>

#include "constants.h"
#include "main.helper.h"
#include "control.model.h"

#define INPUT_BUFF_SIZE 100

int WINAPI ConsoleHandler(DWORD CEvent) {

    switch (CEvent) {
    case CTRL_CLOSE_EVENT:
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
        exit(0);
        break;
    case CTRL_C_EVENT:
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
        exit(0);
        break;
    }
}

int setAppRegistryVars() {
    // TODO change these names to more objective\descritive ones
    TCHAR sharedMemoryWR[] = _T("Pilotion\\sharedMemoryWR");
    TCHAR sharedMemoryRW[] = _T("Pilotion\\sharedMemoryRW");

    return
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("1"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_PLANES, _T("10"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS, _T("30"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_NAMEDPIPE, _T("\\\\.\\pipe\\PilotionControlNamedPipe\0"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_WRITABLE_SHARED_MEMORY, sharedMemoryWR)
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_READABLE_SHARED_MEMORY, sharedMemoryRW)
        && setRegistryVar(REGISTRY_TMP_AVIAO_PATH, REGISTRY_TMP_AVIAO_WRITABLE_SHARED_MEMORY, sharedMemoryRW)
        && setRegistryVar(REGISTRY_TMP_AVIAO_PATH, REGISTRY_TMP_AVIAO_READABLE_SHARED_MEMORY, sharedMemoryWR);
}

// TODO get the Registry var REGISTRY_TMP_CONTROL_STATUS and check if the program is already running (DEPRECATED)
// TODO This should be checked by the namedpipe existance or the shared memory instance existance
void checkControlStatus() {
    TCHAR* appStatus = getRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS);

    if (appStatus == NULL) {
        exit(1);
    }

    if (!wcscmp(appStatus, _T("1"))) {
        wprintf(_T("J� existe um programa control em execu��o.\n"));
        exit(1);
    }
}

void bootstrapInitialSettings() {
    #ifdef UNICODE
        _setmode(_fileno(stdin), _O_WTEXT);
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stderr), _O_WTEXT);
    #endif

    //checkControlStatus();

    if (!setAppRegistryVars()) {
        _wperror(_T("ERROR: O registo das variaveis no Register n�o teve sucesso.\n"));
        exit(1);
    }

    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {
        _wperror(_T("ERROR: O programa n�o consegui carregar o handler da consola.\n"));
        exit(1);
    }
}

void pedirDadosECriarAeroporto(ControlModel* Control) {
    TCHAR nome[INPUT_BUFF_SIZE] = _T("\0");
    int posicaoX = 0, posicaoY = 0;

    wprintf(_T("\nInserir dados do aeroporto\n"));

    wprintf(_T("Nome: ")); fflush(stdout);
    wscanf_s(_T("%99s"), nome, INPUT_BUFF_SIZE);

    wprintf(_T("Posi��o X: ")); fflush(stdout);
    wscanf_s(_T("%i"), &posicaoX, sizeof(int));

    wprintf(_T("Posi��o Y: ")); fflush(stdout);
    wscanf_s(_T("%i"), &posicaoY, sizeof(int));
    
    if (Control->AirportsList == NULL) {
        Control->AirportsList = createAirport(NULL, newString(nome), posicaoX, posicaoY);
        return;
    }

    createAirport(Control->AirportsList, newString(nome), posicaoX, posicaoY);
}

void apresentarMenu() {
    system("cls");

    wprintf(_T("===== Programa Control =====\n"));
    wprintf(_T("Escolha uma das op��es seguintes:\n"));
    wprintf(_T("\ta - Criar um aeroporto;\n"));
    wprintf(_T("\tb - Listar aeroportos disponiveis;\n"));
    wprintf(_T("\t0 - Encerrar programa\n"));

    wprintf(_T("op��o: ")); fflush(stdout);
}

void tratarComandos(TCHAR* comando, ControlModel* Control) {
    if (!wcscmp(comando, _T("a"))) {
        pedirDadosECriarAeroporto(Control);
    }
    else if (!wcscmp(comando, _T("b"))) {
        listarAeroportos(Control->AirportsList);
    }
    else if (!wcscmp(comando, _T("0"))) {
        exit(0);
    }
}

void _tmain() {
    bootstrapInitialSettings();

    TCHAR command[INPUT_BUFF_SIZE] = _T("\0");
    ControlModel Control = initControlModel();

    // routine logic goes here
    while (1) {
        apresentarMenu();

        wscanf_s(_T("%99s"), command, INPUT_BUFF_SIZE);

        tratarComandos(command, &Control);

        system("pause");
    }
    // -----------------------

    setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
}