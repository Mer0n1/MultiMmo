#pragma once
#include "Client.h"

//BridgeClient ���� ������ � ��������� ������ Client � ��� ������� ������� �� ����� ������������ �� ��������� ������ Client

class BridgeClient
{
public:
	BridgeClient();
	static void inisialization(Client* client); //������������� 1 ���

	void MakeDamage(int p_id, int damage); //������� ����
private:
	static Client* client;
};