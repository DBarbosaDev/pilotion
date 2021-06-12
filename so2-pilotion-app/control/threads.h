#pragma once

#include <windows.h>
#include "control.model.h"

DWORD WINAPI controlPlanesConnections(LPVOID Control);
DWORD WINAPI controlThreadConnections(LPVOID Control);
void processaCriacaoPassageiro(LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes);