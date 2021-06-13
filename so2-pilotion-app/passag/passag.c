#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>
#include "passag.model.h"
#include "passag.console.h"
#include "../control/main.helper.h"

#define SIZE_BUFFER 512
#define INPUT_BUFF_SIZE 100

int _tmain(int argc, TCHAR** argv[])
{
// argv[1] = nome
// argv[2] = tempoespera
// argv[3] = siglaerop Partida
// argv[4] = siglaerop Destino

#ifdef UNICODE
    _setmode(_fileno(stdin), _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stderr), _O_WTEXT);
#endif

    if (argc < 5) {
        _tprintf(_T("Numero de argumentos invalido."));
        return 0;
    }
    
    HANDLE hPipe;
    Passageiro* passag = NULL;
    LPTSTR lpvMessage = passag;
    TCHAR  chBuf[SIZE_BUFFER];
    BOOL   fSuccess = FALSE;
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\passengerPipeServer");

    TCHAR nome[INPUT_BUFF_SIZE] = _T("\0");
    wcscpy_s(nome, INPUT_BUFF_SIZE,argv[1]);

    int tempoEspera = atoi(argv[2]);
    TCHAR dados[2][200];
    memset(dados[0], 0, 200);
    memset(dados[1], 0, 200);
    wcscpy_s(dados[0], 200, argv[3]);
    wcscpy_s(dados[1], 200, argv[4]);


    while(1){
        hPipe = CreateFileW(lpszPipename,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_WRITE,
            NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
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
    fSuccess = SetNamedPipeHandleState( hPipe, &dwMode, NULL, NULL);
    if (!fSuccess)
    {
        _tprintf(_T("SetNamedPipeHandleState falhou. Erro=%d\n"), GetLastError());
        return -1;
    }

    Passageiro nPassag = novoPassageiro(-1, tempoEspera, nome, dados);
    lpvMessage = (LPCVOID)&nPassag;

    cbToWrite = sizeof(Passageiro);
    _tprintf(_T("Enviar %d bytes: %s\n"), cbToWrite, lpvMessage);

    fSuccess = WriteFile(
        hPipe,                  // pipe handle 
        lpvMessage,             // message 
        cbToWrite,              // message length 
        &cbWritten,             // bytes written 
        NULL);
    if (!fSuccess)
    {
        _tprintf(_T("Escrita para o pipe falhou. Erro=%d\n"), GetLastError());
        return -1;
    }

    _tprintf(_T("\nEnviar mensagem para o servidor. Receber resposta:\n"));

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

        _tprintf(_T("\"%s\"\n"), chBuf);
    } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

    if (!fSuccess)
    {
        _tprintf(_T("Leitura do pipe falhou. Erro=%d\n"), GetLastError());
        return -1;
    }

    system("pause");
    CloseHandle(hPipe);

    return 0;
}