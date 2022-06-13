#pragma once
#include "JATS.h"
#include "Map.h"
#include "Interface.h"
#include "Textures.h"
#include "Animation.h"

class GameWorld;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void updateHp(int attack) = 0; //принятие дамага
	virtual void update() = 0; //общий метод для всех обьектов

	static void setMap(maps* Map);
	static void setVector2i(Vector2f* pos_);
	static void setTime(float* time_);
	static void setGameWorld(GameWorld* world_);
	static void setView(View* view_);

	static void inisialize(); //пока неопределенный метод

	void setPosX(int x); //установка позиций координат
	void setPosY(int y);
	void addPos(int x_, int y_); //добавление позиции к уже существующей (+=)

	void setSpeed(float);
	void setHp(int);
	void setMaxHp(int);
	void setLife(bool); 

	Vector2f getPos();
	float getSpeed();
	float getSpeedConst();
	int getId();
	int getPid();
	int getWidth();
	int getHeight();
	int getHp();
	int getMaxHp();
	bool getLife();

	Sprite* getModel();
protected:
	string name; //имя игрока
	Texture gamer_texture;

	float time_save, sleeptime; //time_save - перезарядка обычной атаки
	float dx, dy; //координаты направления

	static float* time; //таймер мира
	static TexturesFile* TFPack; //пак текстур
	static maps* Map; //указатель на карту
	static Vector2f* pixelPos; //позиция мышки 
	static View* view;
	static GameWorld* world; 

	float speed; //текущая скорость (движение)
	float speedConst; //характеристика скорости (или макс скорость)
	float x, y; //координаты персонажа
	int dir; //направление персонажа
	int id, pid; //id - персонажа игрока, pid - игровое подайди
	bool life = true; //жизнь
	int hp, max_hp;
	int Width, Height; //ширина высота модельки
public:
	Sprite gamer; //модель персонажа
	HealthBarRpg hb; //полоска хп
};

