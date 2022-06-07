#pragma once
#include "Entity.h"
#include "Attack.h"

struct Group_Rpg //тестова€ структура группы
{
	//каждый состо€щий в группе может пользоватьс€ данными союзников
	string name; //название группы
	vector<Entity*> vec; //список группы
	vector<AttackSystem*> vecAS;
}; 

class GroupSystem
{
public:
	/*
	—истема групп. √руппы позвол€ют 
	*/
	GroupSystem();

	void newGroup(string name, Entity* entity); //добавить новую группу
	void UploadToMap(GameWorld* world); //послать запросы всем кого нет в группе
	void SendRequest(GameWorld* world, int pid); //отправить запрос на добавление в группу 

	void LoadMap(GameWorld* world, string nameMap); //загрузить группы карты

private:
	static vector<Group_Rpg*> groups; //список групп
}; 
