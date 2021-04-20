#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include <signal.h>

#include "constants.h"
#include "main.helper.h"


void signalHandler(int signal) {
    if (signal == SIGTERM) {
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
    }
}

int setAppRegistryVars() {
    return
        setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("1"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_PLANES, _T("10"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS, _T("30"))
        && setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_NAMEDPIPE, _T(""))
        && setRegistryVar(REGISTRY_TMP_AVIAO_PATH, REGISTRY_TMP_AVIAO_WRITABLE_SHARED_MEMORY, _T(""))
        && setRegistryVar(REGISTRY_TMP_AVIAO_PATH, REGISTRY_TMP_AVIAO_READABLE_SHARED_MEMORY, _T(""));
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

void _tmain()
{
    #ifdef UNICODE
        _setmode(_fileno(stdin), _O_WTEXT);
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stderr), _O_WTEXT);
    #endif

    checkControlStatus();
    signal(SIGTERM, signalHandler);

    if (!setAppRegistryVars()) {
        _wperror(_T("ERROR: O registo das variaveis no Register não teve sucesso."));
        exit(1);
    }

    // routine logic goes here


    // -----------------------

    setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_STATUS, _T("0"));
}

