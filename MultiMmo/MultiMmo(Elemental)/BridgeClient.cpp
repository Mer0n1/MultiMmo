﻿#include "BridgeClient.h"

BridgeClient::BridgeClient()
{

}

void BridgeClient::inisialization(Client* client_)
{
	client = client_;
}

void BridgeClient::MakeDamage(int p_id, int damage)
{
	client->Damage(p_id, damage);
}

Client* BridgeClient::client = NULL;