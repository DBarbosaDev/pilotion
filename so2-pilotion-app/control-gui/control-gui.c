#include "framework.h"
#include "control-gui.model.h"
#include "callbacks.h"
#include "main.helper.h"

#define MAX_LOADSTRING 100

ControlModel* pControl;
int test = 0;

BOOL criarJanelas(ControlModel* Control, int nCmdShow)
{
    InterfaceGrafica* GUI = &(Control->gui);
    HWND hJanelaPrincipal;

    RECT retanguloJanela;
    int headerOffset = 40;

    hJanelaPrincipal = extendedCreateWindow(
        &GUI->janelas.principal,
        GUI->janelas.principal.nomeDaClass,
        GUI->janelas.principal.titulo,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        NULL, NULL, GUI->hInstancia, NULL);

    if (!hJanelaPrincipal) { return FALSE; }

    if (GetWindowRect(hJanelaPrincipal, &retanguloJanela)) {
        GUI->janelas.principal.largura = retanguloJanela.right - retanguloJanela.left;
        GUI->janelas.principal.altura = retanguloJanela.bottom - retanguloJanela.top;
    }

    extendedCreateWindow(
        &GUI->janelas.mapa,
        GUI->janelas.mapa.nomeDaClass,
        NULL,
        WS_BORDER | WS_CHILDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, GUI->janelas.principal.altura - headerOffset, GUI->janelas.principal.altura - headerOffset,
        hJanelaPrincipal, NULL, GUI->hInstancia, NULL);

    extendedCreateWindow(
        &GUI->janelas.diario,
        GUI->janelas.diario.nomeDaClass,
        NULL,
        WS_BORDER | WS_CHILDWINDOW,
        GUI->janelas.mapa.largura, 0, GUI->janelas.principal.largura - GUI->janelas.mapa.largura, GUI->janelas.principal.altura * 0.25,
        hJanelaPrincipal, NULL, GUI->hInstancia, NULL);

    extendedCreateWindow(
        &GUI->janelas.controlPannel,
        GUI->janelas.controlPannel.nomeDaClass,
        NULL,
        WS_BORDER | WS_CHILDWINDOW,
        GUI->janelas.mapa.largura, GUI->janelas.diario.altura, GUI->janelas.principal.largura - GUI->janelas.mapa.largura, (GUI->janelas.principal.altura * 0.75) - headerOffset,
        hJanelaPrincipal, NULL, GUI->hInstancia, NULL);

    return TRUE;
}

void criarCamposDoPainelControlo(ControlModel* Control) {
    Janela* janelaParente = &Control->gui.janelas.controlPannel;
    HWND handleJanelaCriada;
    int tamanhoTexto = 20;
    int margem = 20;
    int posicaoY = tamanhoTexto + (margem * 2);
    int larguraDosCampos = janelaParente->largura * 0.5;

    handleJanelaCriada = CreateWindow(L"static", L"Criar aeroporto", WS_CHILDWINDOW, 0, posicaoY, larguraDosCampos, tamanhoTexto, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
    posicaoY += margem + tamanhoTexto;

    handleJanelaCriada = CreateWindow(L"static", L"Nome:", WS_CHILDWINDOW | SS_CENTER, margem, posicaoY, larguraDosCampos - margem, tamanhoTexto, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
    posicaoY += tamanhoTexto;

    handleJanelaCriada = CreateWindow(L"edit", L"", WS_CHILDWINDOW | WS_BORDER, margem, posicaoY, larguraDosCampos - margem, tamanhoTexto, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
    posicaoY += margem + tamanhoTexto;

    handleJanelaCriada = CreateWindow(L"static", L"Posi��o x:", WS_CHILDWINDOW | SS_CENTER, margem, posicaoY, larguraDosCampos - margem, tamanhoTexto, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
    posicaoY += tamanhoTexto;

    handleJanelaCriada = CreateWindow(L"edit", L"", WS_CHILDWINDOW | WS_BORDER, margem, posicaoY, larguraDosCampos - margem, tamanhoTexto, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
    posicaoY += margem + tamanhoTexto;

    handleJanelaCriada = CreateWindow(L"static", L"Posi��o y:", WS_CHILDWINDOW | SS_CENTER, margem, posicaoY, larguraDosCampos - margem, tamanhoTexto, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
    posicaoY += tamanhoTexto;

    handleJanelaCriada = CreateWindow(L"edit", L"", WS_CHILDWINDOW | WS_BORDER, margem, posicaoY, larguraDosCampos - margem, tamanhoTexto, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
    posicaoY += margem + tamanhoTexto;

    handleJanelaCriada = CreateWindow(L"button", L"Criar", WS_CHILDWINDOW | SS_CENTER, margem, posicaoY, larguraDosCampos - margem, tamanhoTexto * 2, janelaParente->handle, NULL, Control->gui.hInstancia, NULL);
    ShowWindow(handleJanelaCriada, Control->gui.nCmdShow);
}

void apresentarJanelas(ControlModel* Control) {
    InterfaceGrafica* GUI = &(Control->gui);

    ShowWindow(GUI->janelas.principal.handle, GUI->nCmdShow);
    
    ShowWindow(GUI->janelas.mapa.handle, GUI->nCmdShow);

    ShowWindow(GUI->janelas.diario.handle, GUI->nCmdShow);

    ShowWindow(GUI->janelas.controlPannel.handle, GUI->nCmdShow);
    criarCamposDoPainelControlo(pControl);

    //GetTextMetrics(hWnd,&lptm);
}

void atualizarJanelas(ControlModel* Control) {
    InterfaceGrafica* GUI = &(Control->gui);

    UpdateWindow(GUI->janelas.principal.handle);
    UpdateWindow(GUI->janelas.mapa.handle);
    UpdateWindow(GUI->janelas.diario.handle);
    UpdateWindow(GUI->janelas.controlPannel.handle);
    //ShowWindow(hWnd, nCmdShow);
    //UpdateWindow(hWnd);

    //GetTextMetrics(hWnd,&lptm);
}

void carregarStrings(ControlModel* Control) {
    WCHAR szTitle[MAX_LOADSTRING];

    LoadStringW(Control->gui.hInstancia, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

    Control->gui.janelas.principal.titulo = newString(szTitle);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProcJanelaPrincipal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Janela janela = pControl->gui.janelas.principal;

    switch (message) {
    case WM_COMMAND:
    {}
    break;
    case WM_CHAR: {
        test++;
        PostMessage(pControl->gui.janelas.controlPannel.handle, WM_PAINT, NULL, NULL);
        PostMessage(hWnd, WM_PAINT, NULL, NULL);
    }
                break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        
        // --------------------------------------------
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcJanelaMapa(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Janela janela = pControl->gui.janelas.mapa;

    switch (message) {
    case WM_COMMAND:
    {}
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...



        // --------------------------------------------
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcJanelaDiario(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Janela janela = pControl->gui.janelas.diario;

    switch (message) {
    case WM_COMMAND:
    {}
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        TextOut(hdc, 0, 0, janela.titulo, wcslen(janela.titulo));
        
        // --------------------------------------------
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcJanelaPainelControlo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Janela janela = pControl->gui.janelas.controlPannel;

    switch (message) {
    case WM_CREATE: {}
    break;
    case WM_CHAR: {
        test++;
        //UpdateWindow(hWnd);
    }
    break;
    case WM_COMMAND:
    {
        test++;
        wprintf(_T("Tut�o Carregado\n"));
        //PostMessage(pControl->gui.janelas.principal.handle, WM_PAINT, NULL, NULL);
        //UpdateWindow(pControl->gui.janelas.principal.handle);
        ShowWindow(hWnd, pControl->gui.nCmdShow);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc 100here...
        TCHAR testS[100] = _T("\0");
        _itow_s(test, testS, 100, 10);
        TextOut(hdc, 250, 400, testS, wcslen(testS));
        TextOut(hdc, 0, 0, janela.titulo, wcslen(janela.titulo));
        
        // --------------------------------------------
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void registarClassesDasJanelas(ControlModel* Control) {
    HINSTANCE hInstancia = Control->gui.hInstancia;
    JanelasAplicacao janelas = Control->gui.janelas;

    registWindowClass(hInstancia, janelas.principal, WndProcJanelaPrincipal, CS_HREDRAW | CS_VREDRAW);
    registWindowClass(hInstancia, janelas.mapa, WndProcJanelaMapa, CS_HREDRAW | CS_VREDRAW);
    registWindowClass(hInstancia, janelas.diario, WndProcJanelaDiario, CS_HREDRAW | CS_VREDRAW);
    registWindowClass(hInstancia, janelas.controlPannel, WndProcJanelaPainelControlo, CS_HREDRAW | CS_VREDRAW);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ControlModel Control = initControlModel();
    pControl = &Control;
    Control.gui.hInstancia = hInstance;
    Control.gui.nCmdShow = nCmdShow;
    
    carregarStrings(&Control);
    passarControlParaContextoCallbacks(&Control);

    registarClassesDasJanelas(&Control);

    if (!criarJanelas(&Control, nCmdShow)) {
        return FALSE;
    }

    apresentarJanelas(&Control);

    HACCEL hAccelTable = LoadAccelerators(Control.gui.hInstancia, MAKEINTRESOURCE(IDC_CONTROLGUI));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}