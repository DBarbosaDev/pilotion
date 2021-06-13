#include "aviao.model.h"
#include "threads_routines.h"
#include "communication.h"
#include "ui.console.h"
#include "../control-gui/communication.model.h"

DWORD WINAPI routinaConfirmacaoConexao(LPVOID aviao) {
	Aviao* pAviao = aviao;

	WaitForSingleObject(pAviao->eventos.handleEventoConfirmacaoConexao, INFINITE);

	memcpy(&pAviao->coordenadasDestino[0], &pAviao->pAviaoStack->coordenadasDestino[0], sizeof(int));
	memcpy(&pAviao->coordenadasDestino[1], &pAviao->pAviaoStack->coordenadasDestino[1], sizeof(int));
	memcpy(&pAviao->coordenadasAtuais[0], &pAviao->pAviaoStack->coordenadasAtuais[0], sizeof(int));
	memcpy(&pAviao->coordenadasAtuais[1], &pAviao->pAviaoStack->coordenadasAtuais[1], sizeof(int));

	pAviao->estadoConexaoComControl = CONNECTION_ACCEPTED;
	SetEvent(pAviao->eventos.handleEventoAeroportosInvalidos);

	return 0;
}

// TODO Pedir novos dados se for ativada
DWORD WINAPI routinaAeroportosInvalidos(LPVOID aviao) {
	Aviao* pAviao = (Aviao*)aviao;

	while (pAviao->estadoConexaoComControl != CONNECTION_ACCEPTED) {
		WaitForSingleObject(pAviao->eventos.handleEventoAeroportosInvalidos, INFINITE);
		
		if (pAviao->estadoConexaoComControl == CONNECTION_ACCEPTED) {
			break;
		}

		pAviao->estadoConexaoComControl = CONNECTION_CORRECTING_PARAMS;

		iniciaUIAeroportos(pAviao->siglaAeroportoPartida, pAviao->siglaAeroportoDestino);
		pAviao->pAviaoStack = adicionaAviaoToStack(pAviao);
		ReleaseSemaphore(pAviao->HandlesAplicacao.hSemaforoParaLeituraItem, 1, NULL);

		ResetEvent(pAviao->eventos.handleEventoAeroportosInvalidos);
	}
    
	return 0;
}

// TODO criar processo de saida do aviao
DWORD WINAPI routinaControloDeSaida(LPVOID aviao) {
	Aviao* pAviao = (Aviao*)aviao;

	/*wprintf(_T("\n>> Thread[%i] para controlar o processo de saida iniciada.\n"), GetCurrentThreadId());
	wprintf(_T(">>> Aguarda evento.\n"));*/
	WaitForSingleObject(pAviao->eventos.handleEventoParaSair, INFINITE);

	return 0;
}