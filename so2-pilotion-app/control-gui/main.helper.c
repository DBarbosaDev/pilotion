#include <windows.h>
#include <tchar.h>
#include <malloc.h>
#include <strsafe.h>
#include "main.helper.h"
#include "constants.h"

TCHAR* newString(TCHAR *text) {
	int textLength = wcslen(text);

	TCHAR* string = malloc((textLength) * sizeof(TCHAR));

	if (string != NULL)
		wcscpy_s(string, (textLength + 1), text); // + 1 to include \0

	return string;
}

int setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, TCHAR* valueData) {
    HKEY registerKey;
    LSTATUS creationStatus = RegCreateKeyEx(HKEY_CURRENT_USER, PATH, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &registerKey, NULL);

    if (creationStatus != ERROR_SUCCESS) {
        wprintf(_T("ERRO: Registo do caminho: %s\n"), PATH);

        RegCloseKey(registerKey);

        return 0;
    }

    creationStatus = RegSetValueEx(registerKey, VALUE_NAME, 0, REG_SZ, valueData, wcslen(valueData) * sizeof(TCHAR));

    if (creationStatus != ERROR_SUCCESS) {
        wprintf(_T("ERRO: Registo da variavel: %s\n"), VALUE_NAME);

        RegCloseKey(registerKey);

        return 0;
    }

    RegCloseKey(registerKey);

    return 1;
}

TCHAR* getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME) {
    DWORD valueDataSize = 0;

    LSTATUS requisitionStatus = RegGetValue(HKEY_CURRENT_USER, PATH, VALUE_NAME, RRF_RT_REG_SZ, NULL, NULL, &valueDataSize);

    if (requisitionStatus != ERROR_SUCCESS) {

        if (requisitionStatus == ERROR_FILE_NOT_FOUND) {
            setRegistryVar(PATH, VALUE_NAME, _T("0"));
            requisitionStatus = RegGetValue(HKEY_CURRENT_USER, PATH, VALUE_NAME, RRF_RT_REG_SZ, NULL, NULL, &valueDataSize);
        }
        else {
            wprintf(_T("ERRO: Insucesso ao obter o tamanho da vari�vel '%s' no caminho '%s'\n"), VALUE_NAME, PATH);
            return NULL;
        }
        
    }

    TCHAR* valueData = malloc(valueDataSize);

    if (!valueData) {
        wprintf(_T("ERRO: Aloca��o de memoria para a variavel em getRegistryVar()\n"));
        return NULL;
    }

    requisitionStatus = RegGetValue(HKEY_CURRENT_USER, PATH, VALUE_NAME, RRF_RT_REG_SZ, NULL, valueData, &valueDataSize);

    if (requisitionStatus != ERROR_SUCCESS) {
        wprintf(_T("ERRO: Insucesso ao obter o valor da vari�vel '%s' do caminho '%s'\n"), VALUE_NAME, PATH);
        return NULL;
    }

    return valueData;
}

int getRegistryVarInt(TCHAR* PATH, TCHAR* VALUE_NAME) {
    TCHAR* registryValue = getRegistryVar(PATH, VALUE_NAME);

    return _wtoi(registryValue, NULL, 10);
}

Aviao* getPlanesStackPointer(HANDLE handle, int numberMaxOfPlanes) {
    Aviao* stack = (Aviao*) MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, (numberMaxOfPlanes * sizeof(Aviao)));

    if (stack == NULL) {
        wprintf(TEXT(">> N�o foi possivel aceder � memoria partilhada. Erro: (%d).\n"),
            GetLastError());
        return NULL;
    }

    return stack;
}

int getIntValueFromSharedMemory(TCHAR* instanceName) {
    HANDLE hMapFile;
    int* pBuf;
    int value;

    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, instanceName);               

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
            GetLastError());
        exit(1);
    }
    pBuf = (int*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));

    if (pBuf == NULL) {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        exit(1);
    }

    value = *pBuf;

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    return value;
}

void setIntValueFromSharedMemory(TCHAR* instanceName, int newValue) {
    HANDLE hMapFile;
    int* pBuf;
    int value;

    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, instanceName);

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
            GetLastError());
        exit(1);
    }
    pBuf = (int*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));

    if (pBuf == NULL) {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        exit(1);
    }

    CopyMemory(pBuf, &newValue, sizeof(int));

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);
    
    UnmapViewOfFile(pBuf);
}

void sendEventByName(TCHAR* eventName) {
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS | EVENT_MODIFY_STATE, FALSE, eventName);

    SetEvent(hEvent);

    CloseHandle(hEvent);
}

HWND extendedCreateWindow(Janela* window, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
    HWND windowHandle = CreateWindowW(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

    window->handle = windowHandle;
    window->handleParente = hWndParent;

    window->posicaoX = x;
    window->posicaoY = y;
    window->largura = nWidth;
    window->altura = nHeight;

    return windowHandle;
}

ATOM registWindowClass(HINSTANCE hInstance, Janela windowProperties, WNDPROC windowProceduresCallback, DWORD csStyle) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.hInstance = hInstance;
    wcex.lpszClassName = windowProperties.nomeDaClass;
    wcex.lpszMenuName = NULL;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CONTROLGUI));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.style = csStyle; //CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = windowProceduresCallback;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void cleanGuiFields(HWND* listOfFields) {
    for (int i = 0; listOfFields[i] != NOT_INITIALIZED_VALUE; i++) {
        SetWindowText(listOfFields[i], _T(""));
    }
}

void freeMemoryNodes(void* node, void* arrayOfNodes[], int arrayOfNodesLength) {
    if (node != NULL) {
        free(node);
    }

    for (int i = 0; i < arrayOfNodesLength; i++) {
        free(arrayOfNodes[i]);
    }
}

void ErrorExit(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}