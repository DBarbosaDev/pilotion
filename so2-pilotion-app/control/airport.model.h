#pragma once

#include <tchar.h>

#include "constants.h"

typedef struct AirportModel {
	TCHAR* name;
	int positionX;
	int positionY;
	struct AirportModel* prox;
} AirportModel;

int isNotInAirportMaxRadius(int positionX, int positionY, int newPositionX, int newPositionY);

AirportModel* createAirport(AirportModel* AirportsList, TCHAR* name, int positionX, int positionY);

AirportModel* getAirportByName(AirportModel* AirportsList, TCHAR* name);