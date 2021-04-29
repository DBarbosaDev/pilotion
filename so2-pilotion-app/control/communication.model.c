
#include "communication.model.h"

CommunicationModel createCommunicationModel(int PID, int MESSAGE_CODE) {
	CommunicationModel communicationModel;

	communicationModel.PID = PID;
	communicationModel.MESSAGE_CODE = MESSAGE_CODE;

	return communicationModel;
}