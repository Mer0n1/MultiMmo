﻿#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SubEntity.h"
#include "Interface.h"
#include "Map.h"
#include "GameWorld.h"
using namespace sf;

class Engine
{
	/*Основной класс - движок, который обьединяет все классы игры*/
public:
	Engine();
	~Engine();
	void start(RenderWindow& window); //запуск движка
	void LoadMap(string name, bool load); //загрузка карты где load - загрузка мобов
private:
	void changeview(); //доп функции карты //загрузка карты

	Player* pr; //класс нашего игрока
	maps Map; //загрузка и вывод текстуры карты а также взаимодействие
	GameWorld world; //класс мира и находящихся в нем игроков
	RpgInterface* Interface;
	GroupSystem* group; //класс групп
	AnimationAttackModule* animation; //класс анимации

	Vector2i pixelPos; //забираем коорд курсора
	Vector2f pos; 
	View view;

	float Timer; //таймер
	float TimeOptimization; //переменная под оптимизацию
	float sleeptime;
	Clock clock;
	Clock OneFrame; //Clock для TimeOptimization
}; 

