#pragma once

#include <windows.h>

#include "resource.h"

#include "airport.model.h"
#include "../aviao/aviao.model.h"
#include "../passag/passag.model.h"

typedef struct SharedMemoryHandles {
	HANDLE planesStack;
	HANDLE planesStackLength;
	HANDLE planesStackSemaphore;

	HANDLE planesStackIndexToWrite;
	HANDLE planesStackIndexToWriteMutex;

	HANDLE planesStackIndexToRead;
	HANDLE planesStackIndexToReadMutex;

	HANDLE planeStackNumItemSemaphore;
	//HANDLE eventAlertPlaneConnection;
} SharedMemoryHandles;

typedef struct SharedMemoryThreads {
	HANDLE handlePlanesConnections;
} SharedMemoryThreads;

typedef struct ApplicationHandles {
	HANDLE keepAliveThread;
	HANDLE connectionsRequestsThread;

	SharedMemoryHandles SharedMemoryHandles;
	SharedMemoryThreads SharedMemoryThreads;
} ApplicationHandles;

typedef struct PassagsLinkedList {
	Passageiro* passag;
	struct PassagsLinkedList* prox;
} PassagsLinkedList;

typedef struct Janela {
	TCHAR* nomeDaClass;
	TCHAR* titulo;
	HWND handle;
	HWND handleParente;

	HWND vetorDeHandlesCamposTexto[256];
	HWND handleBtnDeSubmissao;

	int posicaoX;
	int posicaoY;
	unsigned largura;
	unsigned altura;
} Janela;

typedef struct JanelasAplicacao {
	Janela principal;
	Janela mapa;
	Janela diario;
	Janela controlPannel;
} JanelasAplicacao;

typedef struct InterfaceGrafica {
	HINSTANCE hInstancia;
	int nCmdShow;
	JanelasAplicacao janelas;
} InterfaceGrafica;

typedef struct ControlModel {
	InterfaceGrafica gui;

	AirportModel* AirportsList;
	int airportsListLength;
	int maxAirportsLength;

	Aviao* PlanesList;
	int planesListLength;
	int maxPlanesLength;

	PassagsLinkedList* PassagsList;
	int passagsListLength;

	ApplicationHandles ApplicationHandles;
} ControlModel;

ControlModel initControlModel();

void instanciarMemoriasPartilhadas(ControlModel* Control);
void instanciarIndicesDaMemoriaPartilhada(ControlModel* Control);
void instanciarMutexesSemaforoEventos(ControlModel* Control);
void instanciarThreadsControloDeAvioes(ControlModel* Control);
void instanciarNomeDasJanelasGUI(ControlModel* Control);
void recolherValoresFormularioECriarAeroporto(ControlModel* Control, Janela* janela);