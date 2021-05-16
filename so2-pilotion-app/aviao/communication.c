#pragma once

#include "./communication.h"


Aviao* adicionaAviaoToStack(Aviao* aviao, HANDLE hMapFile) {
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

    CopyMemory(pBuf, aviao, sizeof(Aviao));

    UnmapViewOfFile(pBuf);

    return aviao;
}