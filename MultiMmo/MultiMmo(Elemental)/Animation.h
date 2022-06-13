#pragma once
#include "Textures.h"
#include "StructureEngine.h"
using namespace sf;


class AnimationAttackModule
{
public:
	/* Основа класса - структура AnimationAttack. 
	Задача класса вывод анимации атак для каждого обьекта-Entity тоесть для каждого моба и игрока*/
	AnimationAttackModule();
	~AnimationAttackModule();
	static void setWindow(RenderWindow& window);
	AnimationAttack* newScructureAnimation(); //создает, возвращает и добавляет в список структуру

	//void update();
	void draw(RenderWindow& window);
	void DeleteAnimation(AnimationAttack*);
private:
	Texture changeT, change8;
	Sprite change, change_8; //спрайты тайлов

	static RenderWindow* window;
	static TexturesFile* TFPack;
	static vector<AnimationAttack*> list; //выделить каждому персонажу свою структуру для анимации атаки
};

class AnimationMotionModule
{
	/*
	Модуль класса Animation. Анимации движения

	Анимация движения. Кол во кадров в сек - 15. Метод определяет размеры Width и Height модели, а также
	размеры всей анимации.
	Для добавлении своей анимации можно добавлять кадры как угодно, главное соблюдать дистанцию между кадрами -
	0 пикселей.
	*/
public:
	AnimationMotionModule();
};

class Animation
{
public:
	Animation();

	void animationContol(Sprite& model, Texture& model_); //анимация движения моба

	void setAnimation(bool); //отключить/включить анимацию
private:
	//AnimationMotion AM;
	bool isAnimation;

	Clock clock;
	float time;
	float saveTime;
	int CurCadr;

};


//можно разделить целый класс анимации на его модули - анимация движения, анимация атаки, анимация эффектов