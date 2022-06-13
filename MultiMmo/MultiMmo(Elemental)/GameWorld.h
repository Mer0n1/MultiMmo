#pragma once
#include "Entity.h"
#include "SubEntity.h"
#include "Avatar.h"
#include "UniqueCharacters.h"

class GameWorld
{
/*
Класс мира ( имеет список игроков находящихся в мире)
Отвечает за обновление аватаров (Entity), загрузки Life карты (загрузки Entity-сущностей в карту)
*/
public:
	GameWorld();

	void addEntity(Entity* entity_); //добавить в вектор
	void deleteEntity(int id, int pid); //удалить из вектора
	void update(RenderWindow& window); //обновить персонажей в мире

	/*
	DownloadWorld - загрузка персонажей карты. В методе создаются обьекты мобов где в
	конструкторе этих обьектов через DownloadInformation загружается информации 
	из библиотеки мобов LibraryNPC
	*/
	void DownloadWorld(string nameMap);
	
	Entity* getEntity(int number); //вернем enity
	int getSize(); //вернем кол во обьектов на карте

	void setAvatar(Avatar* avatar, int number); //записать аватар (используется только в клиенте)

private:
	vector<Entity*> entity;
};