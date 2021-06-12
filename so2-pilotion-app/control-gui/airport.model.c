#include <malloc.h>
#include <math.h>

#include "airport.model.h"

int isNotInAirportMaxRadius(int positionX, int positionY, int newPositionX, int newPositionY) {
	return pow((double)newPositionX - (double)positionX, 2.0) + pow((double)newPositionY - (double)positionY, 2.0) > 100;
}

AirportModel* createAirport(HWND windowHandle, AirportModel* AirportsList, TCHAR* name, int positionX, int positionY) {
	AirportModel* Airport = malloc(sizeof(AirportModel));
	AirportModel* auxAirportsList = AirportsList;

	if (Airport == NULL) {
		_wperror(_T("Error: Alocação de memoria em createAirport\n"));
		return NULL;
	}

	Airport->name = name;
	Airport->positionX = positionX;
	Airport->positionY = positionY;
	Airport->prox = NULL;

	if (AirportsList == NULL) {
		MessageBox(windowHandle, _T("Aeroporto criado com sucesso"), _T("Aviso:"), MB_OK);

		AirportsList = Airport;
		return AirportsList;
	}

	while (1) {
		if (!wcscmp(auxAirportsList->name, name)) {
			MessageBox(windowHandle, _T("Já existe um aeroporto com o nome inserido. Tente novamente"), _T("Aviso:"), MB_OK | MB_ICONERROR);
			free(Airport);
			return NULL;
		}

		if (!isNotInAirportMaxRadius(auxAirportsList->positionX, auxAirportsList->positionY, positionX, positionY)) {
			MessageBox(windowHandle, _T("O aeroporto encontra-se dentro do raio de 10 posições de outro aeroporto. Tente outras coordenadas."), _T("Aviso:"), MB_OK | MB_ICONERROR);
			free(Airport);
			return NULL;
		}

		// Break condition
		if (auxAirportsList->prox == NULL) {
			break;
		}

		auxAirportsList = auxAirportsList->prox;
	}

	MessageBox(windowHandle, _T("Aeroporto criado com sucesso"), _T("Aviso:"), MB_OK);

	auxAirportsList->prox = Airport;

	return AirportsList;
}

void listarAeroportos(AirportModel* AirportsList) {
	AirportModel* auxAirportList = AirportsList;

	wprintf(_T("\n>> Aeroportos:\n"));

	if (auxAirportList == NULL) {
		wprintf(_T(">> Ainda não existem aeroportos inseridos no sistema.\n"));
		return;
	}

	while (auxAirportList != NULL) {
		wprintf(
			_T("Nome:%s\t\t| coordenadas:(%i, %i)\n"),
			auxAirportList->name,
			auxAirportList->positionX,
			auxAirportList->positionY
		);

		auxAirportList = auxAirportList->prox;
	}
}

AirportModel* getAirportByName(AirportModel* AirportsList, TCHAR* name) {
	AirportModel* pAirport = NULL;
	AirportModel* auxAirportsList = AirportsList;
	
	if (AirportsList == NULL) {
		return NULL;
	}

	while (auxAirportsList != NULL) {

		if (!wcscmp(auxAirportsList->name, name)) {
			pAirport = auxAirportsList;
			break;
		}

		auxAirportsList = auxAirportsList->prox;
	}

	return pAirport;
}