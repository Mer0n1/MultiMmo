#include "Group.h"
#include "GameWorld.h"
#include "Entity.h"

vector<Group_Rpg*> GroupSystem::groups;

GroupSystem::GroupSystem()
{

}

void GroupSystem::newGroup(string name, Entity* entity)
{
	for (int j = 0; j < groups.size(); j++) //проходимся по всем группам
		if (groups[j]->name == name) { //чтобы понять существует ли такая группа

			for (int c = 0; c < groups[j]->vec.size(); c++) //если такая группа существует и entity в ней то выходим
				if (groups[j]->vec[c]->getPid() == entity->getPid()) return; 

			groups[j]->vec.push_back(entity); return; //если она существует но в ней нет entity добавляем его
		}

	//если группы нет - создаем
	Group_Rpg* newGroup = new Group_Rpg;
	newGroup->name = name;
	newGroup->vec.push_back(entity);

	groups.push_back(newGroup);

}

void GroupSystem::SendRequest(GameWorld* world, int pid)
{
	for (int j = 0; j < world->getSize(); j++)
		if (world->getEntity(j)->getPid() == pid)
			((Enemy*)world->getEntity(j))->setGroup(groups[j]);
}

void GroupSystem::UploadToMap(GameWorld* world)
{
	for (int j = 0; j < groups.size(); j++) //поиск по группам 
		for (int c = 0; c < groups[j]->vec.size(); c++) 
			for (int i = 0; i < world->getSize(); i++)
				if (world->getEntity(i)->getPid() == groups[j]->vec[c]->getPid())
					((Enemy*)world->getEntity(i))->setGroup(groups[j]);

}

void GroupSystem::LoadMap(GameWorld* world, string nameMap)
{
	string way = "maps/" + nameMap + "/Life.xml";
	TiXmlDocument object(way.c_str()); //выбираем какую карту загрузить

	if (!object.LoadFile()) { cout << "Error download map\n"; return; } //test
	object.FirstChildElement("World");

	TiXmlElement* group = object.FirstChildElement("World");
	TiXmlElement* group_ = group->FirstChildElement("group");
	if (group_ == NULL) return; //выход если отсутствуют группы
	
	while (true)
	{
		string name = group_->Attribute("name"); 
		TiXmlElement* pgroup = group_->FirstChildElement("person");
		
		while (true)
		{
			for (int j = 0; j < world->getSize(); j++) 
				if (atoi(pgroup->Attribute("id")) == world->getEntity(j)->getId()) 
					newGroup(name, world->getEntity(j));

			if (!(pgroup = pgroup->NextSiblingElement("person")))
				break; //выход если данных больше нет
		}

		if (!(group_ = group_->NextSiblingElement("group")))
			break; //выход если данных больше нет
	}
	UploadToMap(world); //отправляем запросы на добавление в группу
}
