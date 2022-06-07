#pragma once
#include "JATS.h"
#include "Map.h"
#include "Interface.h"
#include "Textures.h"
#include "Animation.h"

class GameWorld;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void updateXp(int attack) = 0; //�������� ������
	virtual void update() = 0; //����� ����� ��� ���� ��������

	static void setMap(maps* Map);
	static void setVector2i(Vector2f* pos_);
	static void setTime(float* time_);
	static void setGameWorld(GameWorld* world_);
	static void setView(View* view_);

	static void inisialize(); //���� �������������� �����

	void setPosX(int x); //��������� ������� ���������
	void setPosY(int y);
	void addPos(int x_, int y_); //���������� ������� � ��� ������������ (+=)

	void setSpeed(float);
	void setXp(int);
	void setMaxXp(int);
	void setLife(bool); 

	Vector2f getPos();
	float getSpeed();
	float getSpeedConst();
	int getId();
	int getPid();
	int getWidth();
	int getHeight();
	int getXp();
	int getMaxXp();
	bool getLife();

	Sprite* getModel();
protected:
	string name; //��� ������
	Texture gamer_texture;

	float time_save, sleeptime; //time_save - ����������� ������� �����
	float dx, dy; //���������� �����������

	static float* time; //������ ����
	static TexturesFile* TFPack; //��� �������
	static maps* Map; //��������� �� �����
	static Vector2f* pixelPos; //������� ����� 
	static View* view;
	static GameWorld* world; 

	float speed; //������� �������� (��������)
	float speedConst; //�������������� �������� (��� ���� ��������)
	float x, y; //���������� ���������
	int dir; //����������� ���������
	int id, pid; //id - ��������� ������, pid - ������� �������
	bool life = true; //�����
	int xp, max_Xp;
	int Width, Height; //������ ������ ��������
public:
	Sprite gamer; //������ ���������
	HealthBarRpg hb; //������� ��
};

