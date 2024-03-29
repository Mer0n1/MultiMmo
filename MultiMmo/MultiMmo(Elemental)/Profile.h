#pragma once
#include <SFML/Graphics.hpp>
#include "TinyXML/tinyxml.h"
#include <string>
#include "Inventor.h"
#include "Engine.h"
#include "Textures.h"
#include "Redactor.h"
#include <Windows.h>
using namespace sf; 

/*
Профиль аккаунта - здесь можно выбирать персонажей и SA способности для них, иметь доступ к миру и редактору
Задача профиля - показывать параметры аккаунта и изменять их по воле пользователя. 
*/

class Profile
{
public:
	Profile();
	void menu(RenderWindow& window, Client &client); //основное меню
private:
	//menu
	void SA_Inventor(RenderWindow& window, int arg); //инвентари
	void Character_Inventor(RenderWindow& window);
	void SelectingMap(RenderWindow& window); //выбор карты

	//update info
	void updateSA(); //обновить инвентарь персонажей
	void updateCharacter();
	void updateMaps(); //поиск карт в папке maps и их добавление 

	//...
	void saveOptions_SA(int numb, int id); //сохранение настроек выбранных SA 
	void saveOptions_Pr(int idCh_); //сохранение настроек выбранного персонажа

	Engine engine; //класс движка и управления игровым классом игрока
	Inventor inventor; //инвентарь аккаунта
	TexturesFile TT;
	Redactor redactor;

	struct SAElementInv
	{
		Sprite sprite; //спрайт-иконка способности
		Text name; //название абилок (особых способностей в инвентаре)
		Text idSA; //айди по способностей по списку
		int id;
	};

	struct Character
	{
		Sprite frame;
		Sprite sprite;

		int id;
		string name;
		int width, height;
		int hp, maxHp;
		int speed;
		int normAt, At1, At2, At3;
	};

	struct infoMap
	{
		Sprite sprite;
		string name;
		Text text;
	};

	Font font;
	Text text[20];
	Text values[20]; //values text (table - information)
	Texture texture[20];
	Sprite sprite[20];

	//Speciality Attack (SA)
	vector<SAElementInv*> SAElements;
	SAElementInv* CurSAElements[3]; 

	//Characters, account persons
	vector<Character*> PElements;
	Character* CurCh; 

	//maps
	vector<infoMap*> Maps; //названия карт
	infoMap* CurMap;
};