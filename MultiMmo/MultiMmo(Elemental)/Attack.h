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
	static void setAttribute(GameWorld* world, View* view); //инииализирование 
	void inisialization(); //инициализирование данных атаки (разархиварует в локальные переменные этого класса)

	void go_attack(); //Атака через систему AttackSystem
	void attack_start(); //начать атаку

	int getVidE(); 
	bool getLife();
	bool* getLeft(); //вернуть указатель активности атаки
protected:
	void vide(); //особое управление атакой
	void funcAttack(); //прочесть функции атаки
	bool CheckGroup(playerPr* pr, int id); //учесть группу при интерактиве атаки с персонажами

	static GameWorld* world;
	static BridgeClient* client_br;
	static View* view;

	playerPr* pr; //информация об используемом атаке игроке
	CurrentAttack* SAAttack; //информация о атаке (в не совсем структурированном виде)
	type_at type_attack; 
	AnimationAttack* animation;
	Clock clock;

	float time, time_save; //time_save - перезарядка обычной атаки
	float time_attack; //перезарядка для трех особых атак
	int x_attack[100], y_attack[100]; //координаты атаки
	float delay_attack[100]; //задержка атаки (индекс - номер атаки)
	float duration_attack[100]; //продолжительность атаки 
	int distantion; //дистанция атаки
	int QuanTile; //количество тайлов атаки
	string FuncAttack;

	bool BlockOnce = false; //блокирование функции Функция атаки должна срабатывать 1 раз за атаку
	bool life_attack = false; //активна ли атака
	bool Left = false; //активна ли обычная атака

	//--параметры главного ядра
	int id_opponents[20]; //персонажи по которым уже нанесся урон
	float newtimer; //таймер для продолжительнности
	int save_j; //сохраним номер атаки чтобы продолжить ее
	int save_j2; //изменяемый номер атаки (корректируется durationElement)
	int accounting; //номер элемента в id_opponents pid персонажей до которого уже нанесен урон
	bool delayBlock = false; //блоки для атаки и задержки 
	bool active = true; //для блокировки атаки
	float saveS; //переменная под ida и dpsa для установки таймера

	//переменные VidE
    int VidE;
	int saveX_VidE; //сохранение точек рандомного значения в vide 2
	int saveY_VidE;
	int posX_VidE; //фиксирование значений pos.x в видэ 1
	int posY_VidE;
	int BlockVidE; //для какой то единичной инициализации в VidE
	int timeAttack_VidE; //время атаки VidE
	
	/*
	Атака - это заранее подготовленная структура тайлов со своими координатами 
	Тайлы записываются в массив типа [ 0, 1, 2, 3, 4, 5, 6] и расшифровываются на координаты относительно игрока
	 
	Атака делится на 2 типа
	ida - урон в мгновенье 
	do - мгновенный урон
	dpsa - урон в секунду

	На Функции
	ClickPlace - атака на место клика
	Direction - атака в направлении от аватара игрока
	Fixed - атака наносится также как настроена 
	1 - атака отрезком (или атака линией/лучом)
	Left - обычная атака (это не совсем функция поскольку она использует заранее подготовленную структуру атаки)

	Атаке можно задать 2 параметра 
	duration - ее продолжительность. 
	delay - ее задержка
	Параметры можно присваивать любому элементу атаки (любому тайлу)

	VidE - целая группа функций или заранее подготовленных атак которыми могут пользоваться только боты
	1. Например в нее входит новая система функции 1 - создание отрезка, но отрезок можно на этот раз создавать 
	какой угодно, откуда угодно
	2. Можно задать движение и скорость движения ( однако она также создает отрезок направления этого движения,
	можно сказать создание отрезка - целый алгоритм применяемый в VidE)


	Подробнее в руководстве
	*/
};


class AttackSystem
{
private:
	CurrentAttack SAAttack[3]; //для загрузки атаки
	playerPr pr_; //характеристики игрока использующего систему
	AAforASystem* animation; //группа из 4 анимаций на каждую attack
	attack SAt1, SAt2, SAt3, Left; //3 особых атаки (speciality attacks) и Left обычная
	vector<attack> many; //вектор множества атак (сохранение нескольких обьектов в одной атаке) :этап теста
	static AnimationAttackModule* AnimationSystem;

	bool num1 = false, //запуск рендера атаки
		 num2 = false,
		 num3 = false,
		 numLeft = false;
public:
	AttackSystem(AAforASystem*);
    AttackSystem();
	~AttackSystem();
	void go_attack(); //запуск атак если активны

	void inisialization(); //инициализация всех атак
	void addObj(playerPr*, CurrentAttack*); //добавление обьектов в вектор доп атак

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
Краткое руководство
AttackSystem система для настройки обьектов attacks. 
AttackMode (обьект AttackSystem) использует обычную и 3 особых атаки. 
attacks - класс для рендера атаки.

attacks::go_attack() - запуск рендера со стандартными настройками
attacks::attack_() - запуск рендера уже настроенного вне

attacks::inisialization - как 2 конструктор, инициализация атаки (атака берет свойства из сохранений)
*/