#include <malloc.h>

#include "control-gui.model.h"
#include "threads.h"
#include "main.helper.h"

ControlModel initControlModel() {
    ControlModel Control;

    int maxAirportsLength = getRegistryVarInt(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS);
    int maxPlanesLength = getRegistryVarInt(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_PLANES);

    Control.AirportsList = NULL;
    Control.airportsListLength = 0;
    Control.maxAirportsLength = !maxAirportsLength ? 10 : maxAirportsLength;

    Control.PlanesList = NULL;
    Control.planesListLength = 0;
    Control.maxPlanesLength = !maxPlanesLength ? 10 : maxPlanesLength;

    Control.PassagsList = NULL;
    Control.passagsListLength = 0;

    Control.ApplicationHandles.connectionsRequestsThread = NULL;

    instanciarMemoriasPartilhadas(&Control);
    instanciarIndicesDaMemoriaPartilhada(&Control);
    instanciarMutexesSemaforoEventos(&Control);
    instanciarNomeDasJanelasGUI(&Control);

    return Control;
}

void instanciarMemoriasPartilhadas(ControlModel* Control) {
    int numeroMaximoAvioes = Control->maxPlanesLength;

    Control->ApplicationHandles.SharedMemoryHandles.planesStack = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Aviao) * numeroMaximoAvioes,
        SHARED_MEMORY_STACK_PLANES);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStack == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStack (%d).\n"),
            GetLastError());
        return;
    }

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToRead = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int),
        SHARED_MEMORY_STACK_READ_INDEX);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToRead == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStackIndexToRead (%d).\n"),
            GetLastError());
        return;
    }

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWrite = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int),
        SHARED_MEMORY_STACK_WRITE_INDEX);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWrite == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStackIndexToWrite (%d).\n"),
            GetLastError());
        return;
    }

    Control->ApplicationHandles.SharedMemoryHandles.planesStackLength = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int),
        SHARED_MEMORY_STACK_LENGTH_VALUE);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStackLength == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStackLength (%d).\n"),
            GetLastError());
        return;
    }
}

void instanciarIndicesDaMemoriaPartilhada(ControlModel* Control) {
    int* pBuf;
    int indicesIniciais = 0;

    pBuf = (int*)MapViewOfFile(
        Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToRead, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    CopyMemory(pBuf, &indicesIniciais, sizeof(int));
    UnmapViewOfFile(pBuf);

    pBuf = (int*)MapViewOfFile(
        Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWrite, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    CopyMemory(pBuf, &indicesIniciais, sizeof(int));
    UnmapViewOfFile(pBuf);

    pBuf = (int*)MapViewOfFile(
        Control->ApplicationHandles.SharedMemoryHandles.planesStackLength, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    CopyMemory(pBuf, &Control->maxPlanesLength, sizeof(int));
    UnmapViewOfFile(pBuf);
}

void instanciarMutexesSemaforoEventos(ControlModel* Control) {
    Control->ApplicationHandles.SharedMemoryHandles.planesStackSemaphore = CreateSemaphore(
        NULL, Control->maxPlanesLength, Control->maxPlanesLength, SHARED_MEMORY_STACK_SEMAPHORE);

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex = CreateMutex(
        NULL, FALSE, SHARED_MEMORY_STACK_READ_INDEX_MUTEX);

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex = CreateMutex(
        NULL, FALSE, SHARED_MEMORY_STACK_WRITE_INDEX_MUTEX);

    Control->ApplicationHandles.SharedMemoryHandles.eventAlertPlaneConnection = CreateEvent(
        NULL, TRUE, FALSE, EVENT_ALERT_PLANE_CONNECTION);
}

void instanciarThreadsControloDeAvioes(ControlModel* Control) {
    Control->ApplicationHandles.SharedMemoryThreads.handlePlanesConnections = CreateThread(
        NULL, 0, controlPlanesConnections, Control, 0, NULL);
}

void instanciarNomeDasJanelasGUI(ControlModel* Control) {
    Control->gui.janelas.principal.nomeDaClass = WINDOW_CLASS_MAIN_NAME;
    Control->gui.janelas.mapa.nomeDaClass = WINDOW_CLASS_MAP_NAME;

    Control->gui.janelas.diario.nomeDaClass = WINDOW_CLASS_LOGS_NAME;
    Control->gui.janelas.diario.titulo = newString(_T("Diário de eventos."));

    Control->gui.janelas.controlPannel.nomeDaClass = WINDOW_CLASS_PANNEL_NAME;
    Control->gui.janelas.controlPannel.titulo = newString(_T("Painel de controlo"));
}