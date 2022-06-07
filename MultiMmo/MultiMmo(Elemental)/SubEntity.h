#pragma once
#include "Entity.h"
#include "Attack.h"
#include "Group.h"
#include "Animation.h"

class Player : public Entity
{ //����� ������/������������
public:
	Player();
	~Player();

	void update(); //�������� ������
	void updateXp(int attack); //�������� ��

	RechargeAttack* getModuleRA(); //getModuleRechargeAttack - get rech 
	void inisialization(); //����������������� �������������
private:
	void readyPerson(); //��������� �������������� ������
	void downloadSA(int numb, int id); //��������� SA ������
	void interactionWithMap(); //���������� � ������
	void controlPlayer(); //�������� �������

	AttackSystem AttackMode; //������� �����
	Group_Rpg* group = NULL; //������ �� ������ � ������� ������� �����
	
	AttackRatios rations; 
	RechargeAttack* rech;
};


class Enemy : public Entity
{ //����� �����/����������
public:
	Enemy(string name1, int id_, int IDO);  
	~Enemy();

	void update(); //�������� ����������

	void updateXp(int attack); //�������� ��
	void setGroup(Group_Rpg* group); //��������� ������
	void setTether(int DistantionPoint); //��������� ����� ��������
	void setVisibility(int value); //���������� ��������� ��������� ����������
	void setDistantionForOpponent(int value);  //���������� ����. ���������� �� ����������
protected:
	void DownloadInformation(); //�������� ���������� �� ���������� (�� id, pid)

	void go_move(); //�������� �������� ���������
	void ai(); //�� ���������
	bool CheckGroup(int n); //��������� �������������� � ������
	void interactionWithMap(); //���������� � ������
	void controlEnemy(); //�������� ���������
protected:
	AttackSystem AttackMode; //������� ����� (AttackMode � GroupSystem ������ ���� � Entity ������ ����������� �� ���������)
	Group_Rpg* group = NULL; //������ �� ������ � ������� ������� ���
	AttackRatios rations; //������ �� �����

	//��������� �� ����
	int visibility; //��������� ��������� ����
	int DistanceFromOpponent; //������ ���������� �� ������ ������ �������������� ��� 
	int Point[2]; //����� ������ 
	int DistantionPoint; //��������� ��������� ����� �������� �� ����� ������
	bool BlockAi; //������ �������� � ����������� ����������
	bool tether; //false - �� ����� ����� �������� 

	//�������� ����
	int x_move[20], y_move[20]; //����� ���� ������ �������������� �������e���
	bool moving = false; //������� �� ��������?
	bool forward = false, left = false,
		right = false, back = false; //4 ����������� ��������

	//���� � ����������
	int jOp; //����� ���������� � ������ ����
	int pidOp; //pid ���������

	//������
	float controltime; //������ ��� ����������� (������������ � controlEnemy (����������) � ai )
	int IDO; //ID option - ����� �������� ������� � LibraryNPC

	/*
	pidOp - ������� ����������. ������ ����� ��� ����� ���������� ��������� ��� �������, � � ������� ���������� ��� 
	������� ����� � �� �����. ������ ���� ����������� ��� �������, ������ ������� � ����� ������, pidOp �� ����������
	IDO - � ���������� ���������� ����� �������������� ��� � ����� � ��� �� ����, ����� ��������� �� �������� �� ���������
	����������� IDO - �� 0 �� ���������������. 0 - ��������
	*/
};

class SPlayer : public Entity //ServerPlayer (����� �������)
{
public:
	SPlayer(int id, int pid); //��� ������� �������

	void update(); //�������� ������
	void updateXp(int attack); //�������� ��
};

class Mobs : public Enemy
{ //����� ����
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


