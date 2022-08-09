#include "BridgeClient.h"

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

void BridgeClient::ShowAttack(vector<Tile>& tiles, int quan, string type, string identn, int pid)
{
	client->ShowAttack(tiles, quan, type, identn, pid);
}

Client* BridgeClient::client = NULL;