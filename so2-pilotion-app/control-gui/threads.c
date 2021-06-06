#include <tchar.h>

#include "threads.h"
#include "main.helper.h"
#include "constants.h"

DWORD WINAPI controlPlanesConnections(LPVOID Control) {
	ControlModel* pControl = Control;
	int indiceParaEscrita = 0, indiceParaLeitura = 0;

	wprintf(_T("\n\n>> Thread[%i] para o controlo de conexões de aviões, iniciada.\n"), GetCurrentThreadId());

	while (1) {
		WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.eventAlertPlaneConnection, INFINITE);
		

		WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex, INFINITE);
		indiceParaEscrita = getIntValueFromSharedMemory(SHARED_MEMORY_STACK_WRITE_INDEX);
		ReleaseMutex(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex);

		WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex, INFINITE);
		indiceParaLeitura = getIntValueFromSharedMemory(SHARED_MEMORY_STACK_READ_INDEX);

		if (indiceParaLeitura == pControl->maxPlanesLength) {
			setIntValueFromSharedMemory(SHARED_MEMORY_STACK_READ_INDEX, 0);
		}
		else {
			setIntValueFromSharedMemory(SHARED_MEMORY_STACK_READ_INDEX, indiceParaLeitura + 1);
		}

		ReleaseMutex(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex);

		wprintf(_T("\n>> O Aviao[%i] acabou de se conectar com sucesso.\n"), pControl->PlanesList[indiceParaLeitura].PID);
		sendEventByName(pControl->PlanesList[indiceParaLeitura].eventos.nomeEventoConfirmacaoConexao);

		ResetEvent(pControl->ApplicationHandles.SharedMemoryHandles.eventAlertPlaneConnection);
	}
}
