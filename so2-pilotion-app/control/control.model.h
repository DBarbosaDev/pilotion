#pragma once

#include <windows.h>

#include "airport.model.h"
#include "../aviao/aviao.model.h"
#include "../passag/passag.model.h"

typedef struct WorkerThreadHandle {
	HANDLE threadHandle;
	struct WorkerThread* prox;
} WorkerThreadHandle;

typedef struct SharedMemoryHandles {
	HANDLE planesStack;
	HANDLE planesStackLength;
	HANDLE planesStackSemaphore;

	HANDLE planesStackIndexToWrite;
	HANDLE planesStackIndexToWriteMutex;

	HANDLE planesStackIndexToRead;
	HANDLE planesStackIndexToReadMutex;
} SharedMemoryHandles;

typedef struct SharedMemoryThreads {
	HANDLE handlePlanesConnections;
} SharedMemoryThreads;

typedef struct ApplicationHandles {
	WorkerThreadHandle* passagWorkerThreadsHandles;
	HANDLE keepAliveThread;
	HANDLE connectionsRequestsThread;

	SharedMemoryHandles SharedMemoryHandles;
	SharedMemoryThreads SharedMemoryThreads;
} ApplicationHandles;

typedef struct PassagsLinkedList {
	PassagModel* passag;
	struct PassagsLinkedList* prox;
} PassagsLinkedList;

typedef struct ControlModel {

	AirportModel* AirportsList;
	int airportsListLength;
	int maxAirportsLength;

	Aviao* PlanesList;
	int planesListLength;
	int maxPlanesLength;

	PassagsLinkedList* PassagsList;
	int passagsListLength;

	ApplicationHandles ApplicationHandles;

} ControlModel;

ControlModel initControlModel();

void instanciarMemoriasPartilhadas(ControlModel* Control);
void instanciarIndicesDaMemoriaPartilhada(ControlModel* Control);
void instanciarMutexesESemaforo(ControlModel* Control);
void instanciarThreadsControloDeAvioes(ControlModel* Control);