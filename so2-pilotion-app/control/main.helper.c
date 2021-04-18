#include <windows.h>
#include <tchar.h>
#include <malloc.h>

#include "main.helper.h"

TCHAR* newString(TCHAR *text) {
	int textLength = wcslen(text);

	TCHAR* string = malloc((textLength) * sizeof(TCHAR));

	if (string != NULL)
		wcscpy_s(string, (textLength + 1), text); // + 1 to include \0

	return string;
}


void setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, DWORD* valueData) {
    HKEY key;
    LSTATUS creationStatus = RegCreateKeyEx(HKEY_CURRENT_USER, PATH, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);

    if (creationStatus != ERROR_SUCCESS) {
        wprintf(_T("Erro: Registo do caminho: %s\n"), PATH);

        RegCloseKey(key);

        return;
    }

    creationStatus = RegSetValueEx(key, VALUE_NAME, 0, REG_SZ, valueData, sizeof(valueData));

    if (creationStatus != ERROR_SUCCESS) {
        wprintf(_T("Erro: Registo da variavel: %s\n"), VALUE_NAME);
    }


    RegCloseKey(key);
}

void getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME) {

}