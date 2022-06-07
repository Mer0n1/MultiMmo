#pragma once
#include "SubEntity.h"
#include "GameWorld.h"
#include "algorithm.h"

/*
������ UniqueCharacters - ���������� �������������� ����������, ������� ���������� ���� ������ � ��������� �����
*/

class Chara : public Enemy, public attack
{
	bool specialAt;
	float sleeptime;
	int arg; //����� ��������� ������� (� ����� ��������� ����)
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
	float RechargeTeleport, DelaySpAt; //����������� ��������� � �������� ����. �����
	float TimerSp; //������ ��� ����. �����
	Clock clock_Sp; //��� ������
public:
	Sans(int IDO);
	Sans();
	void update();
	void special_attack();
	void attacks();
	void teleport();
};

/*
BadTimeTrio ������������� ��� ����� ������������ ����� attack � ������� AttackSystem ����� ��������� ���� ��������� �
��������� �����
*/