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

int setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, TCHAR* valueData) {
    HKEY registerKey;
    LSTATUS creationStatus = RegCreateKeyEx(HKEY_CURRENT_USER, PATH, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &registerKey, NULL);

    if (creationStatus != ERROR_SUCCESS) {
        wprintf(_T("ERRO: Registo do caminho: %s\n"), PATH);

        RegCloseKey(registerKey);

        return 0;
    }

    creationStatus = RegSetValueEx(registerKey, VALUE_NAME, 0, REG_SZ, valueData, wcslen(valueData) * sizeof(TCHAR));

    if (creationStatus != ERROR_SUCCESS) {
        wprintf(_T("ERRO: Registo da variavel: %s\n"), VALUE_NAME);

        RegCloseKey(registerKey);

        return 0;
    }

    RegCloseKey(registerKey);

    return 1;
}

TCHAR* getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME) {
    DWORD valueDataSize = 0;

    LSTATUS requisitionStatus = RegGetValue(HKEY_CURRENT_USER, PATH, VALUE_NAME, RRF_RT_REG_SZ, NULL, NULL, &valueDataSize);

    if (requisitionStatus != ERROR_SUCCESS) {

        if (requisitionStatus == ERROR_FILE_NOT_FOUND) {
            setRegistryVar(PATH, VALUE_NAME, _T("0"));
            requisitionStatus = RegGetValue(HKEY_CURRENT_USER, PATH, VALUE_NAME, RRF_RT_REG_SZ, NULL, NULL, &valueDataSize);
        }
        else {
            wprintf(_T("ERRO: Insucesso ao obter o tamanho da variável '%s' no caminho '%s'\n"), VALUE_NAME, PATH);
            return NULL;
        }
        
    }

    TCHAR* valueData = malloc(valueDataSize);

    if (!valueData) {
        wprintf(_T("ERRO: Alocação de memoria para a variavel em getRegistryVar()\n"));
        return NULL;
    }

    requisitionStatus = RegGetValue(HKEY_CURRENT_USER, PATH, VALUE_NAME, RRF_RT_REG_SZ, NULL, valueData, &valueDataSize);

    if (requisitionStatus != ERROR_SUCCESS) {
        wprintf(_T("ERRO: Insucesso ao obter o valor da variável '%s' do caminho '%s'\n"), VALUE_NAME, PATH);
        return NULL;
    }

    return valueData;
}

int getRegistryVarInt(TCHAR* PATH, TCHAR* VALUE_NAME) {
    TCHAR* registryValue = getRegistryVar(PATH, VALUE_NAME);

    return _wtoi(registryValue, NULL, 10);
}