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
������� �������� - ����� ����� �������� ���������� � SA ����������� ��� ���, ����� ������ � ���� � ���������
������ ������� - ���������� ��������� �������� � �������� �� �� ���� ������������. 
*/

class Profile
{
public:
	Profile();
	void menu(RenderWindow& window); //�������� ����
private:
	//menu
	void SA_Inventor(RenderWindow& window, int arg); //���������
	void Character_Inventor(RenderWindow& window);
	void SelectingMap(RenderWindow& window); //����� �����

	//update info
	void updateSA(); //�������� ��������� ����������
	void updateCharacter();
	void updateMaps();

	//...
	void saveOptions_SA(int numb, int id); //���������� �������� ��������� SA 
	void saveOptions_Pr(int idCh_); //���������� �������� ���������� ���������

	Engine engine; //����� ������ � ���������� ������� ������� ������
	Inventor inventor; //��������� ��������
	TexturesFile TT;
	Redactor redactor;

	struct SAElementInv
	{
		Sprite sprite; //������-������ �����������
		Text name; //�������� ������ (������ ������������ � ���������)
		Text idSA; //���� �� ������������ �� ������
		int id;
	};

	struct Character
	{
		Sprite frame;
		Sprite sprite;

		int id;
		string name;
		int width, height;
		int xp, maxXp;
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
	vector<infoMap*> Maps; //�������� ����
	infoMap* CurMap;
};