#pragma once
#include "StructureEngine.h"
#include "Entity.h"
#include "BridgeClient.h"

using namespace std;
using namespace sf;

class Client;

class attack
{
public:
	attack();
	attack(playerPr* pr_, CurrentAttack* SAAttack_, AnimationAttack* animation);
	static void setAttribute(GameWorld* world, View* view); //���������������� 
	void inisialization(); //����������������� ������ ����� (������������� � ��������� ���������� ����� ������)

	void go_attack(); //����� ����� ������� AttackSystem
	void attack_start(); //������ �����

	int getVidE(); 
	bool getLife();
	bool* getLeft(); //������� ��������� ���������� �����
protected:
	void vide(); //������ ���������� ������
	void funcAttack(); //�������� ������� �����
	bool CheckGroup(playerPr* pr, int id); //������ ������ ��� ����������� ����� � �����������

	static GameWorld* world;
	static BridgeClient* client_br;
	static View* view;

	playerPr* pr; //���������� �� ������������ ����� ������
	CurrentAttack* SAAttack; //���������� � ����� (� �� ������ ����������������� ����)
	type_at type_attack; 
	AnimationAttack* animation;
	Clock clock;

	float time, time_save; //time_save - ����������� ������� �����
	float time_attack; //����������� ��� ���� ������ ����
	int x_attack[100], y_attack[100]; //���������� �����
	float delay_attack[100]; //�������� ����� (������ - ����� �����)
	float duration_attack[100]; //����������������� ����� 
	int distantion; //��������� �����
	int QuanTile; //���������� ������ �����
	string FuncAttack;

	bool BlockOnce = false; //������������ ������� ������� ����� ������ ����������� 1 ��� �� �����
	bool life_attack = false; //������� �� �����
	bool Left = false; //������� �� ������� �����

	//--��������� �������� ����
	int id_opponents[20]; //��������� �� ������� ��� ������� ����
	float newtimer; //������ ��� ������������������
	int save_j; //�������� ����� ����� ����� ���������� ��
	int save_j2; //���������� ����� ����� (�������������� durationElement)
	int accounting; //����� �������� � id_opponents pid ���������� �� �������� ��� ������� ����
	bool delayBlock = false; //����� ��� ����� � �������� 
	bool active = true; //��� ���������� �����
	float saveS; //���������� ��� ida � dpsa ��� ��������� �������

	//���������� VidE
    int VidE;
	int saveX_VidE; //���������� ����� ���������� �������� � vide 2
	int saveY_VidE;
	int posX_VidE; //������������ �������� pos.x � ���� 1
	int posY_VidE;
	int BlockVidE; //��� ����� �� ��������� ������������� � VidE
	int timeAttack_VidE; //����� ����� VidE
	
	/*
	����� - ��� ������� �������������� ��������� ������ �� ������ ������������ 
	����� ������������ � ������ ���� [ 0, 1, 2, 3, 4, 5, 6] � ���������������� �� ���������� ������������ ������
	 
	����� ������� �� 2 ����
	ida - ���� � ��������� 
	do - ���������� ����
	dpsa - ���� � �������

	�� �������
	ClickPlace - ����� �� ����� �����
	Direction - ����� � ����������� �� ������� ������
	Fixed - ����� ��������� ����� ��� ��������� 
	1 - ����� �������� (��� ����� ������/�����)
	Left - ������� ����� (��� �� ������ ������� ��������� ��� ���������� ������� �������������� ��������� �����)

	����� ����� ������ 2 ��������� 
	duration - �� �����������������. 
	delay - �� ��������
	��������� ����� ����������� ������ �������� ����� (������ �����)

	VidE - ����� ������ ������� ��� ������� �������������� ���� �������� ����� ������������ ������ ����
	1. �������� � ��� ������ ����� ������� ������� 1 - �������� �������, �� ������� ����� �� ���� ��� ��������� 
	����� ������, ������ ������
	2. ����� ������ �������� � �������� �������� ( ������ ��� ����� ������� ������� ����������� ����� ��������,
	����� ������� �������� ������� - ����� �������� ����������� � VidE)


	��������� � �����������
	*/
};


class AttackSystem
{
private:
	CurrentAttack SAAttack[3]; //��� �������� �����
	playerPr pr_; //�������������� ������ ������������� �������
	AAforASystem* animation; //������ �� 4 �������� �� ������ attack
	attack SAt1, SAt2, SAt3, Left; //3 ������ ����� (speciality attacks) � Left �������
	vector<attack> many; //������ ��������� ���� (���������� ���������� �������� � ����� �����) :���� �����
	static AnimationAttackModule* AnimationSystem;

	bool num1 = false, //������ ������� �����
		 num2 = false,
		 num3 = false,
		 numLeft = false;
public:
	AttackSystem(AAforASystem*);
    AttackSystem();
	~AttackSystem();
	void go_attack(); //������ ���� ���� �������

	void inisialization(); //������������� ���� ����
	void addObj(playerPr*, CurrentAttack*); //���������� �������� � ������ ��� ����

	void setNum(int number, bool value); // number [ 1 - 3 ]  4 - numLeft
	void setPr(playerPr* a);
	void setSAAttack(CurrentAttack*, int number); //number [0 - 2]

	playerPr* getPr();
	AAforASystem* getAnimation();
	CurrentAttack* getSAAttack(int number); //number [1 - 3]
	vector<attack> &getMany();
	bool getNum(int number); //number [1 - 3] 4 - numLeft

	static void setAnimationSystem(AnimationAttackModule*);
};

/*
������� �����������
AttackSystem ������� ��� ��������� �������� attacks. 
AttackMode (������ AttackSystem) ���������� ������� � 3 ������ �����. 
attacks - ����� ��� ������� �����.

attacks::go_attack() - ������ ������� �� ������������ �����������
attacks::attack_() - ������ ������� ��� ������������ ���

attacks::inisialization - ��� 2 �����������, ������������� ����� (����� ����� �������� �� ����������)
*/