#pragma once


enum REQUEST_CODE_TYPES {
    REQUEST_QUIT,
    CONNECTION_REQUEST,
    CONNECTION_ACCEPTED,
};

typedef struct CommunicationModel {
	int PID;
    int MESSAGE_CODE;
} CommunicationModel;

CommunicationModel createCommunicationModel(int PID, int MESSAGE_CODE);