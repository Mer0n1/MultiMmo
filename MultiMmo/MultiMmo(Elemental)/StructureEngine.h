#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

enum type_at { Do, dpsa, ida, poisonous }; //��� ����� - ������������

struct AnimationAttack //��������� ��������� ��� ��������
{
	RenderWindow* window;
	Sprite inTile[100];
	Sprite intTile_8[100];

	void Reset() {
		for (int j = 0; j < sizeof(intTile_8) / sizeof(intTile_8[0]); j++) {
			intTile_8[j].setPosition(-10, -10);
			intTile_8[j].setColor(Color::White);
			inTile[j].setPosition(-10, -10);
			inTile[j].setColor(Color::White);
		}
	}
};
struct AAforASystem //��������� ������� �������� ��� ������� ����
{
	AnimationAttack* at1;
	AnimationAttack* at2;
	AnimationAttack* at3;
	AnimationAttack* left;
};

struct CurrentAttack //��������� �����
{
	int id;
	int numberSA;
	float delay[100];
	float duration[100];
	int Tile[100][2];
	int recharge;
	int distantion;
	int QuanityTile;
	int SizeTile = 32; 
	type_at type_attack;
	string FuncAttack;

	//VidE
	int VidE = 0; //type
	int tVide = 0; //time 
};

class Group_Rpg;
struct playerPr { //����� ������ �������������� ��������� ��� ������ attacks
	int id = 1;
	int pid = 2;
	float* x = 0, * y = 0; //������� ������
	float* posX = 0, *posY = 0; //������� �������
	int Height = 0, Width = 0;
	int* xp = 0, * maxXp = 0;

	int* currentAt = 0; //������� �����
	int* normAt;
	int* at1 = 0;
	int* at2 = 0;
	int* at3 = 0;
	Group_Rpg* group = NULL;
};

struct AttackRatios //��������� ������������� ����� (��� "������������ �����")
{
	int currentAt = 0; //���� ������� �����
	int normAt = 5; //���� ������� ������
	int at1 = 40;
	int at2 = 200;
	int at3 = 10;
	int myattack[3] = { 40, 200, 10 };
};