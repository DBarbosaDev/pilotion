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
	HANDLE planesStackSemaphore;

	HANDLE planesStackLength;
	HANDLE planesStackLengthMutex;

	HANDLE planesStackLastFreeIndex;
	HANDLE planesStackLastFreeIndexMutex;
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

	Aviao* PlanesList;
	int planesListLength;

	PassagsLinkedList* PassagsList;
	int passagsListLength;

	ApplicationHandles ApplicationHandles;

} ControlModel;

ControlModel initControlModel();