#pragma once

TCHAR* newString(TCHAR *text);

int setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, TCHAR* valueData);

TCHAR* getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME);

int getRegistryVarInt(TCHAR* PATH, TCHAR* VALUE_NAME);
