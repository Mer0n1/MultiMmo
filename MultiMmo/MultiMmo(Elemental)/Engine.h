#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SubEntity.h"
#include "Interface.h"
#include "Map.h"
#include "GameWorld.h"
using namespace sf;

class Engine
{
	/*
	�������� ����� - ������, ������� ���������� ��� ������ ����
	*/
public:
	Engine();
	~Engine();
	void start(RenderWindow& window); //������ ������
	void LoadMap(string name); //�������� �����
private:
	void changeview(); //��� ������� ����� //�������� �����

	AnimationAttackModule animation; //����� ��������
	Player* pr; //����� ������ ������
	maps Map; //�������� � ����� �������� ����� � ����� ��������������
	GameWorld world; //����� ���� � ����������� � ��� �������
	RpgInterface* Interface;
	GroupSystem group; //����� �����

	Vector2i pixelPos; //�������� ����� �������
	Vector2f pos; 
	View view;

	float Timer; //������
	float sleeptime;
	Clock clock;
}; 

