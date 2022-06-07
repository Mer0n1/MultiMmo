#pragma once
#include "SubEntity.h"
#include "GameWorld.h"
#include "algorithm.h"

/*
«адача UniqueCharacters - добавление индивидуальных персонажей, которые используют свою логику и алгоритмы атаки
*/

class Chara : public Enemy, public attack
{
	bool specialAt;
	float sleeptime;
	int arg; //номер активного обьекта (в цикле множества атак)
	int RechargeSpecialAt;
public:
	Chara(int IDO); 
	Chara();
	void update();
	void special_attack();
	void attacks();
};

class Papyrus : public Enemy, public attack
{
	bool specialAt, num3_;
	int timerNum3, timerSpecialAt;
public:
	Papyrus(int IDO);
	Papyrus();
	void update();
	void special_attack();
	void attacks();
	void attack3();
};

class Sans : public Enemy, public attack
{
	bool teleport_, num4;
	float RechargeTeleport, DelaySpAt; //перезар€дка телепорта и задержка спец. атаки
	float TimerSp; //таймер дл€ спец. атаки
	Clock clock_Sp; //под таймер
public:
	Sans(int IDO);
	Sans();
	void update();
	void special_attack();
	void attacks();
	void teleport();
};

/*
BadTimeTrio демонстрирует как можно использовать класс attack и систему AttackSystem чтобы создавать свои алгоритмы и
поведение атаки
*/