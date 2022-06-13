#pragma once
#include "Client.h"

//BridgeClient дает доступ к основному классу Client и тем методам которым мы можем пользоваться за пределами класса Client

class BridgeClient
{
public:
	BridgeClient();
	static void inisialization(Client* client); //иницилизируем 1 раз

	void MakeDamage(int p_id, int damage); //нанести урон
private:
	static Client* client;
};