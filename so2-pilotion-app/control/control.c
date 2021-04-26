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
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_NAMEDPIPE, _T("PilotionControlNamedPipe\0"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_WRITABLE_SHARED_MEMORY, sharedMemoryWR)
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_READABLE_SHARED_MEMORY, sharedMemoryRW)
        && setRegistryVar(REGISTRY_TMP_AVIAO_PATH, REGISTRY_TMP_AVIAO_WRITABLE_SHARED_MEMORY, sharedMemoryRW)
        && setRegistryVar(REGISTRY_TMP_AVIAO_PATH, REGISTRY_TMP_AVIAO_READABLE_SHARED_MEMORY, sharedMemoryWR);
}

// TODO get the Registry var REGISTRY_TMP_CONTROL_STATUS and check if the program is already running
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

void bootstrapInitialSetings() {
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

void _tmain() {
    bootstrapInitialSetings();

    TCHAR command[INPUT_BUFF_SIZE] = _T("\0");
    ControlModel* Control = initControlModel();

    // routine logic goes here
    while (wcscmp(command, _T("exit"))) {
        wscanf_s(_T("%4s"), command, INPUT_BUFF_SIZE);
    }
    // -----------------------

    setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
}