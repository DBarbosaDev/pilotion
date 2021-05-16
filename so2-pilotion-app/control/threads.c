#include "threads.h"
#include <tchar.h>

DWORD WINAPI controlPlanesConnections(LPVOID Control) {
	ControlModel* pControl = (ControlModel*) Control;
	/*wprintf(_T("\n\n>> Thread[%i] para o controlo de conexões de aviões, iniciada.\n"), GetCurrentThreadId());

	while (1) {
		WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planesStackSemaphore, INFINITE);
		wprintf(_T("Conectou-se uma pessoa\n"));
	}*/
}
