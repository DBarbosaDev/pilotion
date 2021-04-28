#pragma once

#include <windows.h>

#include "airport.model.h"
#include "../aviao/aviao.model.h"
#include "../passag/passag.model.h"

typedef struct WorkerThreadHandle {
	HANDLE threadHandle;
	struct WorkerThread* prox;
} WorkerThreadHandle;

typedef struct ApplicationHandles {
	WorkerThreadHandle* passagWorkerThreadsHandles;
	HANDLE keepAliveThread;
	HANDLE connectionsRequestsThread;
} ApplicationHandles;

typedef struct PassagsLinkedList {
	PassagModel* passag;
	struct PassagsLinkedList* prox;
} PassagsLinkedList;

typedef struct AviaoLinkedList {
	AviaoModel* aviao;
	struct AviaoLinkedList* prox;
} AviaoLinkedList;

typedef struct ControlModel {

	AirportModel* AirportsList;
	int airportsListLength;

	AviaoLinkedList* PlanesList;
	int planesListLength;

	PassagsLinkedList* PassagsList;
	int passagsListLength;

	AviaoLinkedList* PendingAirplanesStack;
	int pendingAirplanesStackLength;

	ApplicationHandles ApplicationHandles;

} ControlModel;

ControlModel initControlModel();