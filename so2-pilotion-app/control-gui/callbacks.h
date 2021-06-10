#pragma once

#include <windows.h>

#include "Resource.h"
#include "control-gui.model.h"

void passarControlParaContextoCallbacks(ControlModel* control);

LRESULT CALLBACK ProcMensagensJanelaPrincipal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);