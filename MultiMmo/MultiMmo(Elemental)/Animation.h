#pragma once
#include "Textures.h"
#include "StructureEngine.h"
using namespace sf;


class AnimationAttackModule
{
public:
	/* ќснова класса - структура AnimationAttack. 
	«адача класса вывод анимации атак дл€ каждого обьекта-Entity тоесть дл€ каждого моба и игрока*/
	AnimationAttackModule();
	~AnimationAttackModule();
	static void setWindow(RenderWindow& window);
	AnimationAttack* newScructureAnimation(); //создает, возвращает и добавл€ет в список структуру

	//void update();
	void draw(RenderWindow& window);
	void DeleteAnimation(AnimationAttack*);
private:
	Texture changeT, change8;
	Sprite change, change_8; //спрайты тайлов

	static RenderWindow* window;
	static TexturesFile* TFPack;
	static vector<AnimationAttack*> list; //выделить каждому персонажу свою структуру дл€ анимации атаки
};

class AnimationMotionModule
{
	/*
	ћодуль класса Animation. јнимации движени€

	јнимаци€ движени€.  ол во кадров в сек - 15. ћетод определ€ет размеры Width и Height модели, а также
	размеры всей анимации.
	ƒл€ добавлении своей анимации можно добавл€ть кадры как угодно, главное соблюдать дистанцию между кадрами -
	0 пикселей.
	*/
public:
	AnimationMotionModule();
};

class Animation
{
public:
	Animation(/*int width_, int height_*/);

	void animationContol(Sprite& model, Texture& model_); //анимаци€ движени€ моба

	void setAnimation(bool); //отключить/включить анимацию
private:
	//AnimationMotion AM;
	bool isAnimation;

	Clock clock;
	float time;
	float saveTime;
	int CurCadr;

};


//можно разделить целый класс анимации на его модули - анимаци€ движени€, анимаци€ атаки, анимаци€ эффектов