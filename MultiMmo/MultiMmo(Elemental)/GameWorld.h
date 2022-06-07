#pragma once
#include "Entity.h"
#include "SubEntity.h"
#include "Avatar.h"
#include "UniqueCharacters.h"

class GameWorld
{
/*
����� ���� ( ����� ������ ������� ����������� � ����)
�������� �� ���������� �������� (Entity), �������� Life ����� (�������� Entity-��������� � �����)
*/
public:
	GameWorld();

	void addEntity(Entity* entity_); //�������� � ������
	void deleteEntity(int id, int pid); //������� �� �������
	void update(RenderWindow& window); //�������� ���������� � ����

	/*
	DownloadWorld - �������� ���������� �����. � ������ ��������� ������� ����� ��� �
	������������ ���� �������� ����� DownloadInformation ����������� ���������� 
	�� ���������� ����� LibraryNPC
	*/
	void DownloadWorld(string nameMap);
	
	Entity* getEntity(int number); //������ enity
	int getSize(); //������ ��� �� �������� �� �����

	void setAvatar(Avatar* avatar, int number); //�������� ������ (������������ ������ � �������)

private:
	vector<Entity*> entity;
};