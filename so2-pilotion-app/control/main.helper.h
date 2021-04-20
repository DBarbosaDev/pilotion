#pragma once

TCHAR* newString(TCHAR *text);

int setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, DWORD* valueData);

TCHAR* getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME);
