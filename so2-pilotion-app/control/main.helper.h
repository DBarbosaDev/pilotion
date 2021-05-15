#pragma once

#include "../aviao/aviao.model.h"

TCHAR* newString(TCHAR *text);

int setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, TCHAR* valueData);

TCHAR* getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME);

int getRegistryVarInt(TCHAR* PATH, TCHAR* VALUE_NAME);

Aviao* getSharedMemoryPlanesStack(HANDLE handle, int numberMaxOfPlanes);
