#pragma once

#include <process.h>
#include "aviao.model.h"

/**
 * @inheritDoc
**/
Aviao novoAviao(DWORD PID, int maxPassag, int coordenadasPorSegundo, TCHAR* dadosAeroporto[]) 
{
	Aviao Aviao;

	Aviao.PID = PID == -1 ? GetCurrentProcessId() : PID;
	Aviao.maxPassag = maxPassag;
	Aviao.coordenadasPorSegundo = coordenadasPorSegundo;
	Aviao.siglaAeroportoPartida = dadosAeroporto[0];
	Aviao.siglaAeroportoDestino = dadosAeroporto[1];

	return Aviao;
}

Passageiro embarcarPassageiro(Passageiro* passageiro, Aviao* aviao)
{
	// int capacidade = sizeof aviao->passageiros / sizeof aviao->passageiros[0];
	//if (aviao->maxPassag == capacidade)
	//{
	//	//send message
	//}

	//if(passageiro->siglaAeroportoDestino == aviao->siglaAeroportoPartida)
		
}
