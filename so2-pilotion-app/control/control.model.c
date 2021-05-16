#include <malloc.h>

#include "control.model.h"


ControlModel initControlModel() {
	ControlModel Control;

	Control.AirportsList = NULL;
	Control.airportsListLength = 0;

	Control.PlanesList = NULL;
	Control.planesListLength = 0;

	Control.PassagsList = NULL;
	Control.passagsListLength = 0;

	Control.ApplicationHandles.connectionsRequestsThread = NULL;

	return Control;
}