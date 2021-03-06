#include <malloc.h>

#include "control-gui.model.h"
#include "threads.h"
#include "main.helper.h"

ControlModel initControlModel() {
    ControlModel Control;

    int maxAirportsLength = getRegistryVarInt(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS);
    int maxPlanesLength = getRegistryVarInt(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_PLANES);

    Control.AirportsList = NULL;
    Control.airportsListLength = 0;
    Control.maxAirportsLength = !maxAirportsLength ? 10 : maxAirportsLength;

    Control.PlanesList = NULL;
    Control.planesListLength = 0;
    Control.maxPlanesLength = !maxPlanesLength ? 10 : maxPlanesLength;

    Control.PassagsList = NULL;
    Control.passagsListLength = 0;

    Control.ApplicationHandles.connectionsRequestsThread = NULL;

    Control.gui.janelas.dimensoesDoMapa[0] = 1000;
    Control.gui.janelas.dimensoesDoMapa[1] = 1000;

    for (int i = 0; i < 256; i++) {
        Control.gui.janelas.controlPannel.vetorDeHandlesCamposTexto[i] = NULL;
        Control.gui.janelas.mapa.vetorDeHandlesCamposTexto[i] = NULL;
        Control.gui.janelas.principal.vetorDeHandlesCamposTexto[i] = NULL;
        Control.gui.janelas.diario.vetorDeHandlesCamposTexto[i] = NULL;
    }

    instanciarMemoriasPartilhadas(&Control);
    instanciarIndicesDaMemoriaPartilhada(&Control);
    instanciarMutexesSemaforoEventos(&Control);
    instanciarNomeDasJanelasGUI(&Control);

    return Control;
}

void instanciarMemoriasPartilhadas(ControlModel* Control) {
    int numeroMaximoAvioes = Control->maxPlanesLength;

    Control->ApplicationHandles.SharedMemoryHandles.planesStack = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Aviao) * numeroMaximoAvioes,
        SHARED_MEMORY_STACK_PLANES);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStack == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStack (%d).\n"),
            GetLastError());
        return;
    }

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToRead = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int),
        SHARED_MEMORY_STACK_READ_INDEX);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToRead == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStackIndexToRead (%d).\n"),
            GetLastError());
        return;
    }

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWrite = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int),
        SHARED_MEMORY_STACK_WRITE_INDEX);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWrite == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStackIndexToWrite (%d).\n"),
            GetLastError());
        return;
    }

    Control->ApplicationHandles.SharedMemoryHandles.planesStackLength = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int),
        SHARED_MEMORY_STACK_LENGTH_VALUE);

    if (Control->ApplicationHandles.SharedMemoryHandles.planesStackLength == NULL) {
        _tprintf(TEXT("Could not create file mapping object planesStackLength (%d).\n"),
            GetLastError());
        return;
    }
}

void instanciarIndicesDaMemoriaPartilhada(ControlModel* Control) {
    int* pBuf;
    int indicesIniciais = 0;

    pBuf = (int*)MapViewOfFile(
        Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToRead, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    CopyMemory(pBuf, &indicesIniciais, sizeof(int));
    UnmapViewOfFile(pBuf);

    pBuf = (int*)MapViewOfFile(
        Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWrite, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    CopyMemory(pBuf, &indicesIniciais, sizeof(int));
    UnmapViewOfFile(pBuf);

    pBuf = (int*)MapViewOfFile(
        Control->ApplicationHandles.SharedMemoryHandles.planesStackLength, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    CopyMemory(pBuf, &Control->maxPlanesLength, sizeof(int));
    UnmapViewOfFile(pBuf);
}

void instanciarMutexesSemaforoEventos(ControlModel* Control) {
    Control->ApplicationHandles.SharedMemoryHandles.planesStackSemaphore = CreateSemaphore(
        NULL, Control->maxPlanesLength, Control->maxPlanesLength, SHARED_MEMORY_STACK_SEMAPHORE);

    Control->ApplicationHandles.SharedMemoryHandles.planeStackNumItemSemaphore = CreateSemaphore(
        NULL, 0, Control->maxPlanesLength, SHARED_MEMORY_STACK_SEMAPHORE_NUM_ITEM);

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToReadMutex = CreateMutex(
        NULL, FALSE, SHARED_MEMORY_STACK_READ_INDEX_MUTEX);

    Control->ApplicationHandles.SharedMemoryHandles.planesStackIndexToWriteMutex = CreateMutex(
        NULL, FALSE, SHARED_MEMORY_STACK_WRITE_INDEX_MUTEX);
}

void instanciarThreadsControloDeAvioes(ControlModel* Control) {
    Control->ApplicationHandles.SharedMemoryThreads.handlePlanesConnections = CreateThread(
        NULL, 0, controlPlanesConnections, Control, 0, NULL);
}

void instanciarNomeDasJanelasGUI(ControlModel* Control) {
    Control->gui.janelas.principal.nomeDaClass = WINDOW_CLASS_MAIN_NAME;
    Control->gui.janelas.mapa.nomeDaClass = WINDOW_CLASS_MAP_NAME;

    Control->gui.janelas.diario.nomeDaClass = WINDOW_CLASS_LOGS_NAME;
    Control->gui.janelas.diario.titulo = newString(_T("Di?rio de eventos."));

    Control->gui.janelas.controlPannel.nomeDaClass = WINDOW_CLASS_PANNEL_NAME;
    Control->gui.janelas.controlPannel.titulo = newString(_T("Painel de controlo"));
}

void recolherValoresFormularioECriarAeroporto(ControlModel* Control, Janela* janela) {
    unsigned int tamanhoPalavra;
    TCHAR* palavras[3];

    for (int i = 0; janela->vetorDeHandlesCamposTexto[i] != NULL; i++) {
        tamanhoPalavra = GetWindowTextLength(janela->vetorDeHandlesCamposTexto[i]) + 1;

        palavras[i] = malloc(sizeof(TCHAR) * tamanhoPalavra);

        GetWindowText(janela->vetorDeHandlesCamposTexto[i], palavras[i], tamanhoPalavra);

        if (i > 0 && (_wtoi(palavras[i]) > Control->gui.janelas.dimensoesDoMapa[0] || _wtoi(palavras[i]) > Control->gui.janelas.dimensoesDoMapa[1])) {
            freeMemoryNodes(NULL, palavras, i + 1);
            MessageBox(janela->handle, _T("As coordenadas x e y devem estar compreendidas entre 0 e 1000."), _T("Aviso:"), MB_OK | MB_ICONINFORMATION);
            return;
        }
    }

    if (Control->AirportsList == NULL) {
        Control->AirportsList = createAirport(janela->handle, NULL, newString(palavras[0]), _wtoi(palavras[1]), _wtoi(palavras[2]));
        Control->airportsListLength += Control->AirportsList != NULL ? 1 : 0;

        cleanGuiFields(janela->vetorDeHandlesCamposTexto);
        InvalidateRect(Control->gui.janelas.mapa.handle, NULL, TRUE);

        freeMemoryNodes(NULL, palavras, 3);

        return;
    }

    AirportModel* resultadoAeroporto = createAirport(janela->handle, Control->AirportsList, newString(palavras[0]), _wtoi(palavras[1]), _wtoi(palavras[2]));
    
    if (resultadoAeroporto != NULL) {
        Control->airportsListLength += 1;

        cleanGuiFields(janela->vetorDeHandlesCamposTexto);
    }
    
    InvalidateRect(Control->gui.janelas.mapa.handle, NULL, TRUE);

    freeMemoryNodes(NULL, palavras, 3);
}