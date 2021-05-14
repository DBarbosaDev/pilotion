#pragma once

#include <windows.h>
#include <tchar.h>

typedef struct PassagModel {
	DWORD PID;

	TCHAR* name;
	TCHAR* airportName;
	TCHAR* destinationAirportName;
	
	// Optional
	int waitingTime;
} PassagModel;

PassagModel newPassagModel(DWORD PID, TCHAR* name, TCHAR* airportName, TCHAR* destinationAirportName, int waitingTime);