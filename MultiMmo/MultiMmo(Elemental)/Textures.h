#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;


class TexturesFile
{
	/*
	���� ������ ��� �������
	������� - ������ ��� ���� �������� ModelCharacter[99] ��� 99 ���� 
	*/
private:
	Texture textureModelCharacter[200];
	Texture textureProfileCharacter[200];
	Texture textureAttack[100];
public:
	Sprite ModelCharacter[200]; //������ ��������� � ����
	Sprite ProfileCharacter[200]; //������ ��������� � �������
	Sprite attack[100]; //�������� ����

	TexturesFile();
};