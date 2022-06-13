#pragma once


struct Avatar
{
	int id, pid;
	int socketDescriptor;
	int hp, maxHp;
	int x, y;
	bool life;
};
/*class Avatar
{
public:
	int id;
	int p_id;
	int id_character;
	int socketDescriptor;

	int xp;
	int maxXp;

	int x, y;

	bool life; //жизнь
}; */