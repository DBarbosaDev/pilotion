#include <processthreadsapi.h>

#include "passag.model.h"

PassagModel newPassagModel(DWORD PID, TCHAR* name, TCHAR* airportName, TCHAR* destinationAirportName, int waitingTime) {
	PassagModel Passag;

	Passag.PID = PID == -1 ? GetCurrentProcessId() : PID;
	Passag.name = name;
	Passag.airportName = airportName;
	Passag.destinationAirportName = destinationAirportName;
	Passag.waitingTime = waitingTime;

	return Passag;
}