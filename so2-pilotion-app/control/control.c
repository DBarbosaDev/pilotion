#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fcntl.h>
#include <conio.h>

#include "control.model.h"
#include "constants.h"
#include "main.helper.h"
#include "threads.h"

#include "../aviao/aviao.model.h"

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
    return
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("1"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_PLANES, _T("10\0"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS, _T("10\0"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_NAMEDPIPE, _T("\\\\.\\pipe\\PilotionControlNamedPipe\0"));
}

// TODO get the Registry var REGISTRY_TMP_CONTROL_STATUS and check if the program is already running (DEPRECATED)
// TODO This should be checked by the namedpipe existance or the shared memory instance existance
void checkControlStatus() {
    TCHAR* appStatus = getRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS);

    if (appStatus == NULL) {
        exit(1);
    }

    if (!wcscmp(appStatus, _T("1"))) {
        wprintf(_T("Já existe um programa control em execução.\n"));
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
        _wperror(_T("ERROR: O registo das variaveis no Register não teve sucesso.\n"));
        exit(1);
    }

    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {
        _wperror(_T("ERROR: O programa não consegui carregar o handler da consola.\n"));
        exit(1);
    }
}

void pedirDadosECriarAeroporto(ControlModel* Control) {
    int numeroMaximoAvioes = getRegistryVarInt(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS);

    if (numeroMaximoAvioes == Control->airportsListLength) {
        wprintf(_T("\n>> O número máximo de aeroportos já foi atingido.\n"));
        return;
    }

    TCHAR nome[INPUT_BUFF_SIZE] = _T("\0");
    int posicaoX = 0, posicaoY = 0;

    wprintf(_T("\nInserir dados do aeroporto\n"));

    wprintf(_T("Nome: ")); fflush(stdout);
    wscanf_s(_T("%99s"), nome, INPUT_BUFF_SIZE);

    wprintf(_T("Posição X: ")); fflush(stdout);
    wscanf_s(_T("%i"), &posicaoX, sizeof(int));

    wprintf(_T("Posição Y: ")); fflush(stdout);
    wscanf_s(_T("%i"), &posicaoY, sizeof(int));
    
    if (Control->AirportsList == NULL) {
        Control->AirportsList = createAirport(NULL, newString(nome), posicaoX, posicaoY);
        Control->airportsListLength += Control->AirportsList != NULL ? 1 : 0;
        return;
    }

    Control->airportsListLength += createAirport(Control->AirportsList, newString(nome), posicaoX, posicaoY) != NULL ? 1 : 0;
}

void apresentarMenu() {
    //system("cls");

    wprintf(_T("\n===== Programa Control =====\n"));
    wprintf(_T("Escolha uma das opções seguintes:\n"));
    wprintf(_T("\ta - Criar um aeroporto;\n"));
    wprintf(_T("\tb - Listar aeroportos disponiveis;\n"));
    wprintf(_T("\t0 - Encerrar programa\n"));

    wprintf(_T("opção: ")); fflush(stdout);
}

void tratarComandos(TCHAR* comando, ControlModel* Control) {
    if (!wcscmp(comando, _T("a"))) {
        pedirDadosECriarAeroporto(Control);
        return;
    }
    if (!wcscmp(comando, _T("b"))) {
        listarAeroportos(Control->AirportsList);
        return;
    }
    if (!wcscmp(comando, _T("0"))) {
        exit(0);
    }

    wprintf(_T("\n>> Comando indisponivel.\n"));
}

void _tmain() {
    bootstrapInitialSettings();

    TCHAR command[INPUT_BUFF_SIZE] = _T("\0");
    ControlModel Control = initControlModel();

    Control.PlanesList = getPlanesStackPointer(
        Control.ApplicationHandles.SharedMemoryHandles.planesStack,
        Control.maxPlanesLength);

    // routine logic goes here
    while (1) {
        apresentarMenu();

        wscanf_s(_T("%99s"), command, INPUT_BUFF_SIZE);

        tratarComandos(command, &Control);
    }
    // -----------------------

    setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
}