#include "Entity.h"
#include "GameWorld.h"

Entity::Entity()
{
}

Entity::~Entity() //ошибка при удалении динамического обьекта
{
	world->deleteEntity(id, pid);
}

void Entity::setPosX(int x_)
{
	x = x_;
}

void Entity::setPosY(int y_)
{
	y = y_;
}

void Entity::addPos(int x_, int y_)
{
	x += x_;
	y += y_;
}

void Entity::setMap(maps* Map_)
{
	Map = Map_;
}


void Entity::setVector2i(Vector2f* pos_)
{
	pixelPos = pos_;
}

void Entity::setView(View* view_)
{
	view = view_;
}

void Entity::setTime(float* time_)
{
	time = time_;
}

void Entity::setGameWorld(GameWorld* world_)
{
	world = world_;
}

void Entity::setSpeed(float q)
{
	speed = q;
}

//void Entity::setDir(int q)
//{
//	dir = q;
//}

void Entity::setHp(int xp_)
{
	hp = xp_;
}

void Entity::setMaxHp(int max)
{
	max_hp = max;
}

void Entity::setLife(bool life_)
{
	life = life_;
}

float Entity::getSpeed()
{
	return speed;
}

float Entity::getSpeedConst()
{
	return speedConst;
}

Vector2f Entity::getPos()
{
	return Vector2f(x,y);
}
//float Entity::getX()
//{
//	return pos.x;
//}
//
//float Entity::getY()
//{
//	return y;
//}

int Entity::getId()
{
	return id;
}

int Entity::getPid()
{
	return pid;
}

int Entity::getWidth()
{
	return Width;
}

int Entity::getHeight()
{
	return Height;
}

int Entity::getHp()
{
	return hp;
}

int Entity::getMaxHp()
{
	return max_hp;
}

bool Entity::getLife()
{
	return life;
}

Sprite* Entity::getModel()
{
	return &gamer;
}

void Entity::inisialize()
{

}

Vector2f* Entity::pixelPos = NULL;
maps* Entity::Map = NULL;
TexturesFile* Entity::TFPack = new TexturesFile;
float* Entity::time = 0;
GameWorld* Entity::world = NULL;
View* Entity::view = NULL;