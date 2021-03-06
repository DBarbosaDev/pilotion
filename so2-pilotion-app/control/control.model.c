#include <malloc.h>

#include "control.model.h"
#include "threads.h"
#include "main.helper.h"

#define SIZE_BUFFER 512

ControlModel initControlModel() {
	ControlModel Control;

    int maxAirportsLength = getRegistryVarInt(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS);
    int maxPlanesLength = getRegistryVarInt(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_PLANES);

	Control.AirportsList = NULL;
	Control.airportsListLength = 0;
    Control.maxAirportsLength = !maxAirportsLength ? 3 : maxAirportsLength;

	Control.PlanesList = NULL;
	Control.planesListLength = 0;
    Control.maxPlanesLength = !maxPlanesLength ? 3 : maxPlanesLength;

	Control.PassagsList = NULL;
	Control.passagsListLength = 0;

	Control.ApplicationHandles.connectionsRequestsThread = NULL;

    instanciarMemoriasPartilhadas(&Control);
    instanciarIndicesDaMemoriaPartilhada(&Control);
    instanciarMutexesSemaforos(&Control);

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

void instanciarMutexesSemaforos(ControlModel* Control) {
    Control->ApplicationHandles.SharedMemoryHandles.planesStackSemaphore = CreateSemaphore(
        NULL, Control->maxPlanesLength, Control->maxPlanesLength, SHARED_MEMORY_STACK_SEMAPHORE);

    Control->ApplicationHandles.SharedMemoryHandles.planeStackNumItemSemaphore = CreateSemaphore(
        NULL, 0, 1, SHARED_MEMORY_STACK_SEMAPHORE_NUM_ITEM);

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex = CreateMutex(
        NULL, FALSE, SHARED_MEMORY_STACK_READ_INDEX_MUTEX);

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex = CreateMutex(
        NULL, FALSE, SHARED_MEMORY_STACK_WRITE_INDEX_MUTEX);

    //Control->ApplicationHandles.SharedMemoryHandles.eventAlertPlaneConnection = CreateEvent(
    //    NULL, TRUE, FALSE, EVENT_ALERT_PLANE_CONNECTION);
}

void instanciarThreadsControloDeAvioes(ControlModel* Control) {
    Control->ApplicationHandles.SharedMemoryThreads.handlePlanesConnections = CreateThread(
        NULL, 0, controlPlanesConnections, Control, 0, NULL);
}


void instanciarNamedPipe(ControlModel* Control)
{
    LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\passengerPipeServer");
    Control->ApplicationHandles.NamedPipeHandles.namedPipe = CreateNamedPipe(
        lpszPipename,             // pipe name 
        PIPE_ACCESS_DUPLEX,       // read/write access 
        PIPE_TYPE_BYTE    |       // message type pipe 
        PIPE_READMODE_BYTE |   // message-read mode 
        PIPE_WAIT,                // blocking mode 
        PIPE_UNLIMITED_INSTANCES, // max. instances  
        SIZE_BUFFER,              // output buffer size 
        SIZE_BUFFER,              // input buffer size 
        0,                        // client time-out 
        NULL                      // default security attribute 
    );

    if (Control->ApplicationHandles.NamedPipeHandles.namedPipe == INVALID_HANDLE_VALUE)
    {
        ErrorExit(TEXT("CreateNamedPipe failed"));
        _tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
        return -1;
    }
}

void instanciarNamedPipeThread(ControlModel* Control)
{
    DWORD  dwThreadId = 0;
    Control->ApplicationHandles.NamedPipeHandles.hThread = CreateThread(
        NULL,              // no security attribute 
        0,                 // default stack size 
        controlThreadConnections,    // thread proc
        (LPVOID)Control,    // thread parameter 
        0,                 // not suspended 
        &dwThreadId);      // returns thread ID 

    if (Control->ApplicationHandles.NamedPipeHandles.hThread == NULL)
    {
        _tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());
        return -1;
    }
    else CloseHandle(Control->ApplicationHandles.NamedPipeHandles.hThread);
}