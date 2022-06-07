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
		window.draw(entity[j]->gamer); //����� ������
		entity[j]->hb.draw(window); //����� ������� ��
	}
}

void GameWorld::setAvatar(Avatar* avatar, int number)
{
	entity[number]->setPosX(avatar->x);
	entity[number]->setPosY(avatar->y);
	entity[number]->setXp(avatar->xp);
	entity[number]->setMaxXp(avatar->maxXp);
	entity[number]->setLife(avatar->life);
}

void GameWorld::DownloadWorld(string nameMap)
{
	string way = "maps/" + nameMap + "/Life.xml";
	TiXmlDocument object(way.c_str()); //�������� ����� 

	if (!object.LoadFile()) { cout << "Error: loading life map failed\n"; return; } //test
	object.FirstChildElement("World");

	TiXmlElement* world = object.FirstChildElement("World");
	TiXmlElement* Mob = world->FirstChildElement("character");
	TiXmlElement* MyPerson = world->FirstChildElement("MyPerson");
	if (!Mob) return; //����� ���� ����� ���

	Enemy* mob = NULL; //���� ��������-������ ��� ���������� � ���
	
	while(true) //���������� ���� ���������� ��� ���� �� �����
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
		
		mob->setXp(atoi(Mob->Attribute("xp")));
		mob->setMaxXp(atoi(Mob->Attribute("maxXp")));
		mob->setPosX(atoi(Mob->Attribute("x")));
		mob->setPosY(atoi(Mob->Attribute("y")));
		mob->setSpeed(atoi(Mob->Attribute("speed")));
		mob->setDistantionForOpponent(atoi(Mob->Attribute("DistanceFromOpponent")));
		mob->setVisibility(atoi(Mob->Attribute("visibility")));
		if (Mob->Attribute("DistantionPoint") != (string)"null") //null - ������ ����������� ����� ��������
			mob->setTether(atoi(Mob->Attribute("DistantionPoint")));

		if (!(Mob = Mob->NextSiblingElement("character")))
			break; //����� ���� ���������� ������ ���
	} 

	if (MyPerson) //��������� ������� ������
	for (int j = 0; j < this->entity.size(); j++)
		if (typeid(*entity[j]).name() == static_cast<string>("class Player"))
		{
			entity[j]->setPosX(atoi(MyPerson->Attribute("x")));
			entity[j]->setPosY(atoi(MyPerson->Attribute("y")));
			break;
		}
}