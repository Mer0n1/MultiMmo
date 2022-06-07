#pragma once
#include "Textures.h"
#include "StructureEngine.h"
using namespace sf;


class AnimationAttackModule
{
public:
	/* ������ ������ - ��������� AnimationAttack. 
	������ ������ ����� �������� ���� ��� ������� �������-Entity ������ ��� ������� ���� � ������*/
	AnimationAttackModule();
	~AnimationAttackModule();
	static void setWindow(RenderWindow& window);
	AnimationAttack* newScructureAnimation(); //�������, ���������� � ��������� � ������ ���������

	//void update();
	void draw(RenderWindow& window);
	void DeleteAnimation(AnimationAttack*);
private:
	Texture changeT, change8;
	Sprite change, change_8; //������� ������

	static RenderWindow* window;
	static TexturesFile* TFPack;
	static vector<AnimationAttack*> list; //�������� ������� ��������� ���� ��������� ��� �������� �����
};

class AnimationMotionModule
{
	/*
	������ ������ Animation. �������� ��������

	�������� ��������. ��� �� ������ � ��� - 15. ����� ���������� ������� Width � Height ������, � �����
	������� ���� ��������.
	��� ���������� ����� �������� ����� ��������� ����� ��� ������, ������� ��������� ��������� ����� ������� -
	0 ��������.
	*/
public:
	AnimationMotionModule();
};

class Animation
{
public:
	Animation(/*int width_, int height_*/);

	void animationContol(Sprite& model, Texture& model_); //�������� �������� ����

	void setAnimation(bool); //���������/�������� ��������
private:
	//AnimationMotion AM;
	bool isAnimation;

	Clock clock;
	float time;
	float saveTime;
	int CurCadr;

};


//����� ��������� ����� ����� �������� �� ��� ������ - �������� ��������, �������� �����, �������� ��������