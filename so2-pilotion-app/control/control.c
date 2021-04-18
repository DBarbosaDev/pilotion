#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <fcntl.h>

#include "constants.h"
#include "main.helper.h"

int _tmain()
{
    #ifdef UNICODE
        _setmode(_fileno(stdin), _O_WTEXT);
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stderr), _O_WTEXT);
    #endif
    
    setRegistryVar(REGISTRY_TMP_CONTROL_PATH, REGISTRY_TMP_CONTROL_MAX_AIRPORTS, _T("30"));
}

