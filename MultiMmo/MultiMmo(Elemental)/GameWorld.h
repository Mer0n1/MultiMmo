#pragma once
#include "Entity.h"
#include "SubEntity.h"
#include "Avatar.h"
#include "UniqueCharacters.h"

class GameWorld
{
/*
 ласс мира ( имеет список игроков наход€щихс€ в мире)
ќтвечает за обновление аватаров (Entity), загрузки Life карты (загрузки Entity-сущностей в карту)
*/
public:
	GameWorld();

	void addEntity(Entity* entity_); //добавить в вектор
	void deleteEntity(int id, int pid); //удалить из вектора
	void update(RenderWindow& window); //обновить персонажей в мире

	/*
	DownloadWorld - загрузка персонажей карты. ¬ методе создаютс€ обьекты мобов где в
	конструкторе этих обьектов через DownloadInformation загружаетс€ информации 
	из библиотеки мобов LibraryNPC
	*/
	void DownloadWorld(string nameMap);
	
	Entity* getEntity(int number); //вернем enity
	int getSize(); //вернем кол во обьектов на карте

	void setAvatar(Avatar* avatar, int number); //записать аватар (используетс€ только в клиенте)

private:
	vector<Entity*> entity;
};