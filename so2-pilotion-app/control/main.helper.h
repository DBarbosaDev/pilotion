#pragma once

#include "../aviao/aviao.model.h"

TCHAR* newString(TCHAR *text);

int setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, TCHAR* valueData);

TCHAR* getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME);

int getRegistryVarInt(TCHAR* PATH, TCHAR* VALUE_NAME);

Aviao* getPlanesStackPointer(HANDLE handle, int numberMaxOfPlanes);

int getIntValueFromSharedMemory(TCHAR* instanceName);
void setIntValueFromSharedMemory(TCHAR* instanceName, int newValue);

void sendEventByName(TCHAR* eventName);

void ErrorExit(LPTSTR lpszFunction);
