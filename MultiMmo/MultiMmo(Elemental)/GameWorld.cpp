#include "GameWorld.h"
#include "TinyXML/tinyxml.h"

GameWorld::GameWorld()
{

}

void GameWorld::addEntity(Entity* entity_)
{
	entity.push_back(entity_);
}

void GameWorld::deleteEntity(int id, int pid)
{
	for (int j = 0; j < entity.size(); j++)
		if (entity[j]->getPid() == pid)
		{
			entity.erase(entity.begin() + j);
			break;
		}
}

Entity* GameWorld::getEntity(int number)
{
	return entity[number];
}

int GameWorld::getSize()
{
	return entity.size();
}

void GameWorld::update(RenderWindow& window)
{
	for (int j = 0; j < entity.size(); j++) {

		if (!entity[j]->getLife()) 
		{ delete entity[j]; continue; }
		
		entity[j]->update();
		window.draw(entity[j]->gamer); //вывод модели
		entity[j]->hb.draw(window); //вывод полоски хп
	}
}

void GameWorld::setAvatar(Avatar* avatar, int number)
{
	entity[number]->setPosX(avatar->x);
	entity[number]->setPosY(avatar->y);
	entity[number]->setHp(avatar->hp);
	entity[number]->setMaxHp(avatar->maxHp);
	entity[number]->setLife(avatar->life);
}

void GameWorld::DownloadWorld(string nameMap)
{
	string way = "maps/" + nameMap + "/Life.xml";
	TiXmlDocument object(way.c_str()); //выбираем карту 

	if (!object.LoadFile()) { cout << "Error: loading life map failed\n"; return; } //test
	object.FirstChildElement("World");

	TiXmlElement* world = object.FirstChildElement("World");
	TiXmlElement* Mob = world->FirstChildElement("character");
	TiXmlElement* MyPerson = world->FirstChildElement("MyPerson");
	if (!Mob) return; //выход если мобов нет

	Enemy* mob = NULL; //наше существо-обьект для добавления в мир
	
	while(true) //добавление всех персонажей что есть на карте
	{
		string data = Mob->Attribute("type");
		
		if (data == "mobs")
			mob = new Mobs(Mob->Attribute("name"), atoi(Mob->Attribute("id")), atoi(Mob->Attribute("IDO")));
		else
		if (data == "BadTimeTrio")
		{
			if (Mob->Attribute("name") == (string)"sans") mob = new Sans();
			if (Mob->Attribute("name") == (string)"papyrus") mob = new Papyrus();
			if (Mob->Attribute("name") == (string)"chara") mob = new Chara();
			if (mob == NULL) mob = new Enemy(Mob->Attribute("name"), atoi(Mob->Attribute("id")), atoi(Mob->Attribute("IDO")));
		}
		else
			mob = new Enemy(Mob->Attribute("name"), atoi(Mob->Attribute("id")), atoi(Mob->Attribute("IDO")));
		
		mob->setHp(atoi(Mob->Attribute("hp")));
		mob->setMaxHp(atoi(Mob->Attribute("maxHp")));
		mob->setPosX(atoi(Mob->Attribute("x")));
		mob->setPosY(atoi(Mob->Attribute("y")));
		mob->setSpeed(atoi(Mob->Attribute("speed")));
		mob->setDistantionForOpponent(atoi(Mob->Attribute("DistanceFromOpponent")));
		mob->setVisibility(atoi(Mob->Attribute("visibility")));
		if (Mob->Attribute("DistantionPoint") != (string)"null") //null - значит отсутствует точка привязки
			mob->setTether(atoi(Mob->Attribute("DistantionPoint")));

		if (!(Mob = Mob->NextSiblingElement("character")))
			break; //выход если персонажей больше нет
	} 

	if (MyPerson) //установка позиции игрока
	for (int j = 0; j < this->entity.size(); j++)
		if (typeid(*entity[j]).name() == static_cast<string>("class Player"))
		{
			entity[j]->setPosX(atoi(MyPerson->Attribute("x")));
			entity[j]->setPosY(atoi(MyPerson->Attribute("y")));
			break;
		}
}