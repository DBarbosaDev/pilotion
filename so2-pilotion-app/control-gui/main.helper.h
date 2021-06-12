#pragma once

#include "control-gui.model.h"
#include "../aviao/aviao.model.h"

TCHAR* newString(TCHAR *text);
int setRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME, TCHAR* valueData);
TCHAR* getRegistryVar(TCHAR* PATH, TCHAR* VALUE_NAME);
int getRegistryVarInt(TCHAR* PATH, TCHAR* VALUE_NAME);
Aviao* getPlanesStackPointer(HANDLE handle, int numberMaxOfPlanes);
int getIntValueFromSharedMemory(TCHAR* instanceName);
void setIntValueFromSharedMemory(TCHAR* instanceName, int newValue);
void sendEventByName(TCHAR* eventName);
HWND extendedCreateWindow(Janela* window, LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,int x,int y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
ATOM registWindowClass(HINSTANCE hInstance, Janela windowProperties, WNDPROC windowProceduresCallback, DWORD csStyle);
void cleanGuiFields(HWND* listOfFields);