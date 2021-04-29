#pragma once

#include <windows.h>
#include <tchar.h>

typedef struct AviaoModel {
	DWORD PID;
	int maxPassag;

	int velocityInPositionsPerSecond;
	
	TCHAR* airportName;
	TCHAR* destinationAirportName;

} AviaoModel;

AviaoModel newAviaoModel(DWORD PID, int maxPassag, int velocityInPositionsPerSecond, TCHAR* airportName, TCHAR* destinationAirportName);