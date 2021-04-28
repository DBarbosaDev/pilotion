#include <malloc.h>
#include <math.h>

#include "airport.model.h"

int isNotInAirportMaxRadius(int positionX, int positionY, int newPositionX, int newPositionY) {
	return pow((double)newPositionX - (double)positionX, 2.0) + pow((double)newPositionY - (double)positionY, 2.0) > 100;
}

AirportModel* createAirport(AirportModel* AirportsList, TCHAR* name, int positionX, int positionY) {
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
		AirportsList = Airport;
		return AirportsList;
	}

	while (1) {
		if (!wcscmp(auxAirportsList->name, name)) {
			wprintf(_T("Já existe um aeroporto com o nome %s\n"), name);
			free(Airport);
			return NULL;
		}

		if (!isNotInAirportMaxRadius(auxAirportsList->positionX, auxAirportsList->positionY, positionX, positionY)) {
			wprintf(_T("Existe um aeroporto(%s) num raio de 10 posições\n"), auxAirportsList->name);
			free(Airport);
			return NULL;
		}

		// Break condition
		if (auxAirportsList->prox == NULL) {
			break;
		}

		auxAirportsList = auxAirportsList->prox;
	}

	auxAirportsList->prox = Airport;

	return AirportsList;
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