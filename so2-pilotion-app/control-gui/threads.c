#include <tchar.h>

#include "threads.h"
#include "main.helper.h"
#include "constants.h"

DWORD WINAPI controlPlanesConnections(LPVOID Control) {
	ControlModel* pControl = Control;
	int indiceParaEscrita = 0, indiceParaLeitura = 0;

	wprintf(_T("\n\n>> Thread[%i] para o controlo de conexões de aviões, iniciada.\n"), GetCurrentThreadId());

	while (1) {

		DWORD waitForNumItems = WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planeStackNumItemSemaphore, INFINITE);

		switch (waitForNumItems) {
		case WAIT_OBJECT_0:
			//WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex, INFINITE);
			//indiceParaEscrita = getIntValueFromSharedMemory(SHARED_MEMORY_STACK_WRITE_INDEX);
			//ReleaseMutex(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex);

			WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex, INFINITE);
			indiceParaLeitura = getIntValueFromSharedMemory(SHARED_MEMORY_STACK_READ_INDEX);

			if (indiceParaLeitura == pControl->maxPlanesLength)
				setIntValueFromSharedMemory(SHARED_MEMORY_STACK_READ_INDEX, 0);
			else
				setIntValueFromSharedMemory(SHARED_MEMORY_STACK_READ_INDEX, indiceParaLeitura + 1);

			ReleaseMutex(pControl->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex);
			wprintf(_T("\n>> O Aviao[%i] acabou de se conectar com sucesso.\n"), pControl->PlanesList[indiceParaLeitura].PID);

			break;
		case WAIT_TIMEOUT:
			_tprintf(TEXT("Could not receive any sign of num items change. %d\n"), GetLastError());
			break;
		}

		ReleaseSemaphore(pControl->ApplicationHandles.SharedMemoryHandles.planeStackNumItemSemaphore, -1, NULL);
	}
}
