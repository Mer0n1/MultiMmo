#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;


class TexturesFile
{
	/*
	База данных для текстур
	Принцип - индекс это айди например ModelCharacter[99] это 99 айди 
	*/
private:
	Texture textureModelCharacter[200];
	Texture textureProfileCharacter[200];
	Texture textureAttack[100];
public:
	Sprite ModelCharacter[200]; //модель персонажа в игре
	Sprite ProfileCharacter[200]; //модель персонажа в профиле
	Sprite attack[100]; //текстуры атак

	TexturesFile();
};