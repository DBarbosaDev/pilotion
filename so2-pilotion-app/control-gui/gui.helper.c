#include "gui.helper.h"

void drawAirports(ControlModel* Control, HDC hdc, HDC hdcMem) {
	int tamanhoIcon = 50;
	int tamanhoTexto = 16;
	AirportModel* auxAirportsLinkedList = Control->AirportsList;
	Janela janelaMapa = Control->gui.janelas.mapa;

	BITMAP iconBitMap;
	HWND hJanelaIcon;
	HGDIOBJ oldBitmap;

	/*	Ajustes entre o tam da janela e a dimensão do mapa
		para a coordenada x_convertido
			x_convertido = (posicao * larguraDaJanela)/(dimensaoMapaLargura)
		para a coordenada y_convertido
			y_convertido = (posicao * alturaDaJanela)/(dimensaoMapaAltura)
	*/ 
	int coordenadasConvertidasNaEscala[2] = { 0, 0 };

	while (auxAirportsLinkedList != NULL) {
		coordenadasConvertidasNaEscala[0] = (auxAirportsLinkedList->positionX * (janelaMapa.largura - tamanhoIcon)) / (Control->gui.janelas.dimensoesDoMapa[0]);
		coordenadasConvertidasNaEscala[1] = ((auxAirportsLinkedList->positionY + tamanhoTexto) * (janelaMapa.altura - tamanhoIcon - tamanhoTexto)) / (Control->gui.janelas.dimensoesDoMapa[1]);


		DrawIcon(hdcMem, coordenadasConvertidasNaEscala[0], coordenadasConvertidasNaEscala[1], Control->gui.janelas.hIconAeroporto);
		TextOut(hdcMem, coordenadasConvertidasNaEscala[0], coordenadasConvertidasNaEscala[1] - tamanhoTexto, auxAirportsLinkedList->name, wcslen(auxAirportsLinkedList->name));

		auxAirportsLinkedList = auxAirportsLinkedList->prox;
	}
}
