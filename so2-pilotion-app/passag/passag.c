#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "passag.model.h"
#include "passag.console.h"
#include "../control/main.helper.h"

#define SIZE_BUFFER 512
#define INPUT_BUFF_SIZE 100

int _tmain(int argc, char* argv[])
{
#ifdef UNICODE
    _setmode(_fileno(stdin), _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stderr), _O_WTEXT);
#endif

    HANDLE hPipe;
    Passageiro* passag = NULL;
    LPTSTR lpvMessage = passag;
    TCHAR  chBuf[SIZE_BUFFER];
    BOOL   fSuccess = FALSE;
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\passengerPipeServer");

    TCHAR dados[2][200];
    for (size_t i = 0; i < 2; i++)
        memset(dados[i], 0, 200);

    TCHAR nome[INPUT_BUFF_SIZE];
    memset(nome, 0, INPUT_BUFF_SIZE);
    int tempoEspera = 0;

    while(1){
        hPipe = CreateFileW(lpszPipename,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_WRITE,
            NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
        );
        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        if (GetLastError() != ERROR_PIPE_BUSY)
        {
            ErrorExit(TEXT("Nao foi possivel abrir a pipe."));
            _tprintf(TEXT("Nao foi possivel abrir a pipe. GLE=%d\n"), GetLastError());
            return -1;
        }

        if (!WaitNamedPipe(lpszPipename, 20000))
        {
            _tprintf("Nao foi possivel abrir a pipe: 20 second wait timed out.");
            return -1;
        }
    }

    dwMode = PIPE_READMODE_BYTE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        _tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
        return -1;
    }

    iniciaUI(&nome, &dados, &tempoEspera);
    Passageiro nPassag = novoPassageiro(-1, nome, dados[0], dados[1], tempoEspera);
    lpvMessage = (LPCVOID)&nPassag;

    cbToWrite = sizeof(Passageiro);
    _tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

    fSuccess = WriteFile(
        hPipe,                  // pipe handle 
        lpvMessage,             // message 
        cbToWrite,              // message length 
        &cbWritten,             // bytes written 
        NULL);
    if (!fSuccess)
    {
        _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
        return -1;
    }

    _tprintf("\nMessage sent to server, receiving reply as follows:\n");

    do
    {
        // Read from the pipe. 

        fSuccess = ReadFile(
            hPipe,    // pipe handle 
            chBuf,    // buffer to receive reply 
            SIZE_BUFFER * sizeof(TCHAR),  // size of buffer 
            &cbRead,  // number of bytes read 
            NULL);    // not overlapped 

        if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
            break;

        _tprintf(TEXT("\"%s\"\n"), chBuf);
    } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

    if (!fSuccess)
    {
        _tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
        return -1;
    }

    _tprintf("\n<End of message, press ENTER to terminate connection and exit>");

    CloseHandle(hPipe);

    return 0;
}