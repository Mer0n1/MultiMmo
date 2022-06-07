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
	void menu(RenderWindow& window); //���� �������� ��� ���������

private:
	void world(RenderWindow& window); //�������� �����

	void SpriteMove(int x, int y); //������������ �������� (����� ��� �������� �����������)
	void CoorDistantion(RectangleShape& Ysh, RectangleShape& Ysh2, RectangleShape& Xsh, RectangleShape& Xsh2); //������ ������� �����
	//void loading(int id, string name); //�������� ������������ �����
	void CreateAttack(); //������� �����

	RenderWindow redactor; //2 ����� ��� 2 ����� (����� ����������� ���������)
	
	Texture newtex[20]; //�������� ��� �������� � �������
	Sprite interface_1; //�������� ���������
	Sprite *change; //������� ������
	Sprite Create, Load; //������� ������ �������� � �������� �����
	Sprite player; //������ �������

	Font font;
	Text text[35]; //������ �������
	Text arrows[10]; //��������� ������������
	
	bool wind; //���� �������� �� ���� ������ ��� ���? 
	int width, height; //������ � ������ ������� (���������)

	//��������������
	int OCH; //���������� ����� (��)
	int QuanTile; //��� �� ��������� ������
	int distance; //��������� ���������
	bool acc; //��������
	int recharge; //�����������
	int SizeTile; //������ ������
	string type_attack;
	string FuncAttack; //������� �����

	int _Price; //��������� ������ (��������� �����)
	int _PriceTypeAttack; //��������� ������ ������ � ����� �����

	//menu2
	View view;
	int newX = 0, newY = 0; //��������� ������� ������� ��� ������� ������ ����� ����������� �����
	int coordinate[100][2]; //������ ��� ������ ������ � �� ���������
	int redCo[2]; //���������� ����������� �����
	int numC; //����� ����������� ����� � ������� 
	float delay_[100]; //�������� ����������� �����
	float duration_[100]; //����������������� �. �����
};

