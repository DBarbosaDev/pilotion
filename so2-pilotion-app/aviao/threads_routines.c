#include "aviao.model.h"
#include "threads_routines.h"
#include "../control/communication.model.h"

DWORD WINAPI routinaConfirmacaoConexao(LPVOID aviao) {
	Aviao* pAviao = (Aviao*) aviao;

	wprintf(_T("\n>> Thread[%i] para confirmação de conexão iniciada\n"), GetCurrentThreadId());
	wprintf(_T(">>> Aguarda evento.\n"));

	WaitForSingleObject(pAviao->eventos.handleEventoConfirmacaoConexao, INFINITE);

	wprintf(_T("\n>> Conexão aceite com sucesso.\n"), GetCurrentThreadId());

	pAviao->estadoConexaoComControl = CONNECTION_ACCEPTED;
	//CloseHandle(pAviao->Threads.hConfirmacaoConexao);

	return 0;
}

// TODO Pedir novos dados se for ativada
DWORD WINAPI routinaAeroportosInvalidos(LPVOID aviao) {
	Aviao* pAviao = (Aviao*)aviao;

	wprintf(_T("\n>> Thread[%i] para alterar dados aeroportos inválidos iniciada.\n"), GetCurrentThreadId());
	wprintf(_T(">>> Aguarda evento.\n"));
	WaitForSingleObject(pAviao->eventos.handleEventoAeroportosInvalidos, INFINITE);
	wprintf(_T("\n>> O controlo informa que os dados do aeroporto estão inválidos.\n"), GetCurrentThreadId());


	return 0;
}

// TODO criar processo de saida do aviao
DWORD WINAPI routinaControloDeSaida(LPVOID aviao) {
	Aviao* pAviao = (Aviao*)aviao;

	wprintf(_T("\n>> Thread[%i] para controlar o processo de saida iniciada.\n"), GetCurrentThreadId());
	wprintf(_T(">>> Aguarda evento.\n"));
	WaitForSingleObject(pAviao->eventos.handleEventoParaSair, INFINITE);

	return 0;
}