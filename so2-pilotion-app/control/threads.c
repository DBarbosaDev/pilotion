#include <tchar.h>

#include "threads.h"
#include "main.helper.h"
#include "constants.h"

#define SIZE_BUFFER 512

DWORD WINAPI controlPlanesConnections(LPVOID Control)
{
    ControlModel *pControl = Control;
    int indiceParaEscrita = 0, indiceParaLeitura = 0;

    wprintf(_T("\n\n>> Thread[%i] para o controlo de conex�es de avi�es, iniciada.\n"), GetCurrentThreadId());

    while (1)
    {

        DWORD waitForNumItems = WaitForSingleObject(pControl->ApplicationHandles.SharedMemoryHandles.planeStackNumItemSemaphore, INFINITE);

        switch (waitForNumItems)
        {
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

DWORD WINAPI controlThreadConnections(LPVOID Control)
{
    ControlModel* ControlTyped = (ControlModel*)Control;
    HANDLE hHeap = GetProcessHeap();
    Passageiro* pchRequest = (Passageiro *)HeapAlloc(hHeap, 0, SIZE_BUFFER * sizeof(Passageiro));
    Passageiro* pchReply = (Passageiro*)HeapAlloc(hHeap, 0, SIZE_BUFFER * sizeof(Passageiro));

    DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
    BOOL fSuccess = FALSE;
    HANDLE hPipe = NULL;

    if (Control == NULL)
    {
        wprintf(_T("\nERROR - Pipe Server Failure:\n"));
        wprintf(_T("Thread got an unexpected NULL value in lpvParam.\n"));
        wprintf(_T("Thread exitting.\n"));
        if (pchReply != NULL)
            HeapFree(hHeap, 0, pchReply);
        if (pchRequest != NULL)
            HeapFree(hHeap, 0, pchRequest);
        return (DWORD)-1;
    }

    if (pchRequest == NULL)
    {
        wprintf(_T("\nERROR - Pipe Server Failure:\n"));
        wprintf(_T("Thread got an unexpected NULL heap allocation.\n"));
        wprintf(_T("Thread exitting.\n"));
        if (pchReply != NULL)
            HeapFree(hHeap, 0, pchReply);
        return (DWORD)-1;
    }

    if (pchReply == NULL)
    {
        wprintf(_T("\nERROR - Pipe Server Failure:\n"));
        wprintf(_T("   Thread got an unexpected NULL heap allocation.\n"));
        wprintf(_T("   Thread exitting.\n"));
        if (pchRequest != NULL)
            HeapFree(hHeap, 0, pchRequest);
        return (DWORD)-1;
    }

    wprintf(_T("Thread criada, a processar e receber mensagens...\n"));
    hPipe = (HANDLE)ControlTyped->ApplicationHandles.NamedPipeHandles.namedPipe;

    while (1)
    {
        fSuccess = ReadFile(
            hPipe,                       // handle to pipe
            pchRequest,                  // buffer to receive data
            SIZE_BUFFER * sizeof(TCHAR), // size of buffer
            &cbBytesRead,                // number of bytes read
            NULL);                       // not overlapped I/O

        if (!fSuccess || cbBytesRead == 0)
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
                _tprintf(TEXT("Thread: cliente offline.\n"));
            else
                _tprintf(TEXT("Thread ReadFile falhado, GLE=%d.\n"), GetLastError());
            break;
        }

        // Process the incoming message.
        processaCriacaoPassageiro(pchRequest, pchReply, &cbReplyBytes, ControlTyped);

        // Write the reply to the pipe.
        fSuccess = WriteFile(
            hPipe,        // handle to pipe
            pchReply,     // buffer to write from
            cbReplyBytes, // number of bytes to write
            &cbWritten,   // number of bytes written
            NULL);        // not overlapped I/O

        if (!fSuccess || cbReplyBytes != cbWritten)
        {
            _tprintf(TEXT("Thread WriteFile falhado, GLE=%d.\n"), GetLastError());
            break;
        }
    }
}

void processaCriacaoPassageiro(LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes, ControlModel* Control)
{
    // This routine is a simple function to print the client request to the console
    // and populate the reply buffer with a default data string. This is where you
    // would put the actual client request processing code that runs in the context
    // of an instance thread. Keep in mind the main thread will continue to wait for
    // and receive other client connections while the instance thread is working.
    Passageiro* novoPassageiro = (Passageiro*)pchRequest;
    
    //@todo: consultar os aeroportos, e verificar se o aeroporto de destino e de partida existem, caso existam
    // adicionar a stack de passageiros do control
    Control->PassagsList->passag = novoPassageiro;

    // Check the outgoing message to make sure it's not too long for the buffer.
    if (FAILED(StringCchCopy(pchReply, SIZE_BUFFER, TEXT("default answer from server"))))
    {
        *pchBytes = 0;
        pchReply[0] = 0;
        wprintf(_T("StringCchCopy failed, no outgoing message.\n"));
        return;
    }
    *pchBytes = (lstrlen(pchReply) + 1) * sizeof(TCHAR);
}