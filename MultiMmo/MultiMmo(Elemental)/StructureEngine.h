#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

enum type_at { Do, dpsa, ida, poisonous }; //тип атаки - перечисления

struct AnimationAttack //временная структура под анимацию
{
	RenderWindow* window;
	Sprite intTile_8[100];

	void Reset() {
		for (int j = 0; j < sizeof(intTile_8) / sizeof(intTile_8[0]); j++) {
			intTile_8[j].setPosition(-100, -100);
			intTile_8[j].setColor(Color::White);
		}
	}
};
struct AAforASystem //структура системы анимаций для системы атак
{
	AnimationAttack* at1;
	AnimationAttack* at2;
	AnimationAttack* at3;
	AnimationAttack* left;
};

struct CurrentAttack //структура атаки
{
	int id;
	int numberSA;
	float delay[100];
	float duration[100];
	int Tile[100][2]; //начальные тайлы
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
struct playerPr { //берем важные характеристики персонажа для класса attacks
	int id = 1;
	int pid = 2;
	float* x = 0, * y = 0; //позиция игрока
	float* posX = 0, *posY = 0; //позиция курсора
	int Height = 0, Width = 0;
	int* hp = 0, * maxHp = 0;

	int* currentAt = 0; //текущая атака
	int* normAt;
	int* at1 = 0;
	int* at2 = 0;
	int* at3 = 0;
	Group_Rpg* group = NULL;
};

struct AttackRatios //Структура характеристик атаки (или "Коэффициенты атаки")
{
	int currentAt = 0; //урон текущей атаки
	int normAt = 5; //урон обычной атакой
	int at1 = 40;
	int at2 = 200;
	int at3 = 10;
	int myattack[3] = { 40, 200, 10 };
};