#pragma once
#include "Entity.h"
#include "Attack.h"

struct Group_Rpg //�������� ��������� ������
{
	//������ ��������� � ������ ����� ������������ ������� ���������
	string name; //�������� ������
	vector<Entity*> vec; //������ ������
	vector<AttackSystem*> vecAS;
}; 

class GroupSystem
{
public:
	/*
	������� �����. ������ ��������� 
	*/
	GroupSystem();

	void newGroup(string name, Entity* entity); //�������� ����� ������
	void UploadToMap(GameWorld* world); //������� ������� ���� ���� ��� � ������
	void SendRequest(GameWorld* world, int pid); //��������� ������ �� ���������� � ������ 

	void LoadMap(GameWorld* world, string nameMap); //��������� ������ �����

private:
	static vector<Group_Rpg*> groups; //������ �����
}; 
