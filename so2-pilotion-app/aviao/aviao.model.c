#include <process.h>
#include "aviao.model.h"

AviaoModel newAviaoModel(DWORD PID, int maxPassag, int velocityInPositionsPerSecond, TCHAR* airportName, TCHAR* destinationAirportName) {
	AviaoModel Aviao;

	Aviao.PID = PID == -1 ? GetCurrentProcessId() : PID;
	Aviao.maxPassag = maxPassag;
	Aviao.velocityInPositionsPerSecond = velocityInPositionsPerSecond;
	Aviao.airportName = airportName;
	Aviao.destinationAirportName = destinationAirportName;

	return Aviao;
}