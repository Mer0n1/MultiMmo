#pragma once
#include "TinyXML/tinyxml.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include "JATS.h"
#include "StructureEngine.h"
#include "Attack.h"
using namespace sf;

class Redactor
{
public:
	Redactor();
	void menu(RenderWindow& window); //меню настроек для редактора

private:
	void world(RenderWindow& window); //редактор атаки

	void SpriteMove(int x, int y); //передвижение спрайтов (нужно для удобного перемещения)
	void CoorDistantion(RectangleShape& Ysh, RectangleShape& Ysh2, RectangleShape& Xsh, RectangleShape& Xsh2); //задает границы атаки
	//void loading(int id, string name); //загрузка существующей атаки
	void CreateAttack(); //создать атаку

	RenderWindow redactor; //2 форма или 2 экран (экран внутреннего редактора)
	
	Texture newtex[20]; //текстуры для загрузки в спрайты
	Sprite interface_1; //основной интерфейс
	Sprite *change; //спрайты тайлов
	Sprite Create, Load; //спрайты кнопок загрузки и создания атаки
	Sprite player; //спрайт аватара

	Font font;
	Text text[35]; //прочие надписи
	Text arrows[10]; //стрелочки переключения
	
	bool wind; //клик проходит по окну тайлов или нет? 
	int width, height; //ширина и высота Аватара (персонажа)

	//характеристики
	int OCH; //количество очков (ОЧ)
	int QuanTile; //кол во купленных клеток
	int distance; //купленная дистанция
	bool acc; //задержка
	int recharge; //перезарядка
	int SizeTile; //размер тайлов
	string type_attack;
	string FuncAttack; //функция атаки

	int _Price; //стоимость сборки (структуры атаки)
	int _PriceTypeAttack; //стоимость сборки вместе с типом атаки

	//menu2
	View view;
	int newX = 0, newY = 0; //изменение позиции курсора под позицию тайлов после перемещения карты
	int coordinate[100][2]; //массив для записи тайлов и их координат
	int redCo[2]; //координата выделенного тайла
	int numC; //номер выделенного тайла в массиве 
	float delay_[100]; //задержка выделенного тайла
	float duration_[100]; //продолжительность в. тайла
};

