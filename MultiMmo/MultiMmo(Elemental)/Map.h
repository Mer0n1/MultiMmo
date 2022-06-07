#pragma once
#include <SFML/Graphics.hpp>
#include "TinyXML/tinyxml.h"
#include "JATS.h"
using namespace sf;


class maps
{ //редактор создания карты
private:
	Sprite menu; 
	Texture menu0;
	Sprite change; 
	Texture change0;

	Texture general[100]; //общая текстура
	Sprite general_[100]; //спрайт - сборка 

	int WIDTH_MAP; //размер карты ширина
	int HEIGHT_MAP; //размер карты высота/длина
	int SizeTileMap; //размер тайла относительно стандарта 32

	int TileMap[100][100]; //основной слой
	int TileFonMap[100][100]; //задний фон

public:
	maps();

	bool interactionMap(int x, int y, int Height, int Width); //интерактив персонажа с картой
	void limitMap(float& x, float& y, View &view); //интерактив с границами карты

	void TileMapEdit(string fail); //загрузка карты
	void draw_map(RenderWindow& window);
};