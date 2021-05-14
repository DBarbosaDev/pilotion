#pragma once

#include <process.h>
#include "aviao.model.h"
#include "../control/main.helper.h"

/**
 * @inheritDoc
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR dadosAeroporto[2][200])
{
	Aviao Aviao;

	Aviao.PID = PID == -1 ? GetCurrentProcessId() : PID;
	Aviao.maxPassag = maxPassag;
	Aviao.coordenadasPorSegundo = coordenadasPorSegundo;
	Aviao.siglaAeroportoPartida = newString(dadosAeroporto[0]);
	Aviao.siglaAeroportoDestino = newString(dadosAeroporto[1]);

	return Aviao;
}

/**
 * @inheritDoc
**/
//Passageiro *embarcarPassageiro(Passageiro *passageiro, Aviao *aviao)
//{
//	int capacidade = sizeof(aviao->passageiros) / sizeof(Passageiro);
//	if (aviao->maxPassag == capacidade)
//	{
//		// enviar mensagem para avisar que a maxima capacidade foi atingida
//	}
//
//	if (passageiro->siglaAeroportoDestino == aviao->siglaAeroportoPartida)
//		aviao->passageiros[capacidade - 1] = passageiro;
//
//	return passageiro;
//}
