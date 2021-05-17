#pragma once

#include "./communication.h"
#include "../control/main.helper.h"
#include "../control/constants.h"


Aviao* adicionaAviaoToStack(Aviao* aviao, HANDLE hMapFile) {
    Aviao* pAviaoStackNode = NULL;
    HANDLE hMutexIndiceEscrita = OpenMutex(MUTEX_ALL_ACCESS | MUTEX_MODIFY_STATE, FALSE, SHARED_MEMORY_STACK_WRITE_INDEX_MUTEX);
    int indiceDeEscrita = 0;

	Aviao *pBuf = (Aviao*)MapViewOfFile(hMapFile, // handle to map object
        FILE_MAP_ALL_ACCESS,  // read/write permission
        0,
        0,
        sizeof(Aviao) * aviao->nrMaximoDeAvioes);

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());
        CloseHandle(hMapFile);

        return NULL;
    }

    WaitForSingleObject(hMutexIndiceEscrita, INFINITE);

    indiceDeEscrita = getIntValueFromSharedMemory(SHARED_MEMORY_STACK_WRITE_INDEX);

    if (indiceDeEscrita == aviao->nrMaximoDeAvioes) {
        setIntValueFromSharedMemory(SHARED_MEMORY_STACK_WRITE_INDEX, 0);
    }
    else {
        setIntValueFromSharedMemory(SHARED_MEMORY_STACK_WRITE_INDEX, indiceDeEscrita + 1);
    }

    ReleaseMutex(hMutexIndiceEscrita);

    memset((pBuf + indiceDeEscrita), 0, sizeof(Aviao));
    CopyMemory(pBuf + indiceDeEscrita, aviao, sizeof(Aviao));

    return aviao;
}