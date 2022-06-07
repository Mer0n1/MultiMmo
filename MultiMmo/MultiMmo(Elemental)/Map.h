#pragma once
#include <SFML/Graphics.hpp>
#include "TinyXML/tinyxml.h"
#include "JATS.h"
using namespace sf;


class maps
{ //�������� �������� �����
private:
	Sprite menu; 
	Texture menu0;
	Sprite change; 
	Texture change0;

	Texture general[100]; //����� ��������
	Sprite general_[100]; //������ - ������ 

	int WIDTH_MAP; //������ ����� ������
	int HEIGHT_MAP; //������ ����� ������/�����
	int SizeTileMap; //������ ����� ������������ ��������� 32

	int TileMap[100][100]; //�������� ����
	int TileFonMap[100][100]; //������ ���

public:
	maps();

	bool interactionMap(int x, int y, int Height, int Width); //���������� ��������� � ������
	void limitMap(float& x, float& y, View &view); //���������� � ��������� �����

	void TileMapEdit(string fail); //�������� �����
	void draw_map(RenderWindow& window);
};