#include <tchar.h>

#include "threads.h"
#include "main.helper.h"
#include "constants.h"
#include "airport.model.h"

DWORD WINAPI controlPlanesConnections(LPVOID Control) {
	ControlModel* pControl = (ControlModel*) Control;
	int indiceParaEscrita = 0, indiceParaLeitura = 0;

	Aviao* aviao;

	wprintf(_T("\n\n>> Thread[%i] para o controlo de conexões de aviões, iniciada.\n"), GetCurrentThreadId());

	while (1) {
		switch (WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planeStackNumItemSemaphore, INFINITE)) {
		case WAIT_OBJECT_0:
			WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex, INFINITE);
			indiceParaEscrita = getIntValueFromSharedMemory(SHARED_MEMORY_STACK_WRITE_INDEX);

			getIndexesFromSharedMemory(
				pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex,
				&indiceParaLeitura, pControl->maxPlanesLength
			);

			aviao = pControl->PlanesList + indiceParaLeitura;
			AirportModel* aeroportoDestino = getAirportByName(pControl->AirportsList, aviao->siglaAeroportoDestino);
			AirportModel* aeroportoPartida = getAirportByName(pControl->AirportsList, aviao->siglaAeroportoPartida);
			if (aeroportoDestino == NULL || aeroportoPartida == NULL) {
				sendEventByName(aviao->eventos.nomeEventoAeroportosInvalidos);
			}
			else {
				aviao->coordenadasAtuais[0] = aeroportoPartida->positionX;
				aviao->coordenadasAtuais[1] = aeroportoPartida->positionY;
				aviao->coordenadasDestino[0] = aeroportoDestino->positionX;
				aviao->coordenadasDestino[1] = aeroportoDestino->positionY;

				sendEventByName(aviao->eventos.nomeEventoConfirmacaoConexao);
			}

			ReleaseMutex(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex);
			break;
		default:
			break;
		}
		ReleaseSemaphore(pControl->ApplicationHandles.SharedMemoryHandles.planeStackNumItemSemaphore, -1, NULL);
	}
}
