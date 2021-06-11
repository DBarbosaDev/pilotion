#include <process.h>

#include "passag.model.h"

Passageiro novoPassageiro(DWORD PID, TCHAR* nome, TCHAR* siglaAeroportoPartida, TCHAR* siglaAeroportoDestino, int tempoEspera) {
	Passageiro Passag;

	Passag.pid = PID == -1 ? GetCurrentProcessId() : PID;
	Passag.nome = nome;
	Passag.siglaAeroportoPartida = siglaAeroportoPartida;
	Passag.siglaAeroportoDestino = siglaAeroportoDestino;
	Passag.tempoEspera = tempoEspera;
	Passag.embarcado = false;

	return Passag;
}