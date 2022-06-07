#pragma once
#include "Entity.h"
#include "Attack.h"
#include "Group.h"
#include "Animation.h"

class Player : public Entity
{ //класс игрока/пользователя
public:
	Player();
	~Player();

	void update(); //обновить игрока
	void updateXp(int attack); //обновить хп

	RechargeAttack* getModuleRA(); //getModuleRechargeAttack - get rech 
	void inisialization(); //переинициализация характеристик
private:
	void readyPerson(); //загрузить характеристики игрока
	void downloadSA(int numb, int id); //загрузить SA игрока
	void interactionWithMap(); //интерактив с картой
	void controlPlayer(); //контроль аватара

	AttackSystem AttackMode; //система атаки
	Group_Rpg* group = NULL; //ссылка на группу в которой состоит игрок
	
	AttackRatios rations; 
	RechargeAttack* rech;
};


class Enemy : public Entity
{ //класс врага/противника
public:
	Enemy(string name1, int id_, int IDO);  
	~Enemy();

	void update(); //обновить противника

	void updateXp(int attack); //обновить хп
	void setGroup(Group_Rpg* group); //установка группы
	void setTether(int DistantionPoint); //установки точки привязки
	void setVisibility(int value); //установить дальность видимости противника
	void setDistantionForOpponent(int value);  //установить фикс. расстояние от противника
protected:
	void DownloadInformation(); //загрузка информации из библоитеки (по id, pid)

	void go_move(); //начинает движение персонажа
	void ai(); //ии персонажа
	bool CheckGroup(int n); //проверить принадлежность к группе
	void interactionWithMap(); //интерактив с картой
	void controlEnemy(); //контроль персонажа
protected:
	AttackSystem AttackMode; //система атаки (AttackMode и GroupSystem должны быть в Entity однако архитектура не позволяет)
	Group_Rpg* group = NULL; //ссылка на группу в которой состоит бот
	AttackRatios rations; //данные об уроне

	//настройки ИИ бота
	int visibility; //дальность видимости цели
	int DistanceFromOpponent; //какого расстояния от игрока должен придерживаться бот 
	int Point[2]; //точка спавна 
	int DistantionPoint; //дистанция насколько можно отходить от точки спавна
	bool BlockAi; //блочит движение с сохранением дистанцией
	bool tether; //false - не имеет точки привязки 

	//движение бота
	int x_move[20], y_move[20]; //точки куда должно осуществляться перемещeние
	bool moving = false; //активно ли движение?
	bool forward = false, left = false,
		right = false, back = false; //4 направления движения

	//инфа о противнике
	int jOp; //номер противника в списке мира
	int pidOp; //pid оппонента

	//прочее
	float controltime; //таймер для оптимизации (используется в controlEnemy (управлении) и ai )
	int IDO; //ID option - номер варианта аватара в LibraryNPC

	/*
	pidOp - подайди противника. Всегда когда нпс видит противника сохраняет его подайди, и с подайди включается его 
	система атаки и ИИ атаки. Однако если посторонний нпс союзник, тоесть состоит в одной группе, pidOp не отмечается
	IDO - в библиотеке существуют много разноуровневых нпс с одним и тем же айди, чтобы различать их допустим по сложности
	использется IDO - от 0 до неограниченного. 0 - стандарт
	*/
};

class SPlayer : public Entity //ServerPlayer (игрок сервера)
{
public:
	SPlayer(int id, int pid); //под сетевых игроков

	void update(); //обновить игрока
	void updateXp(int attack); //обновить хп
};

class Mobs : public Enemy
{ //класс моба
public:
	Mobs(string name, int id, int pid);
	~Mobs();
	void Control();
	void updateXp(int);
	void update();
private:
	Texture model_;
	Animation animation_;
	Clock clock;

	float time;
	float saveTime;
	int CurCadr = 1;
};


