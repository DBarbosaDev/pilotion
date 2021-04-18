#pragma once

TCHAR* newString(TCHAR *text);

void setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, DWORD* valueData);
void getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME);
