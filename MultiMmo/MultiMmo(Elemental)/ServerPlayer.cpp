#include "SubEntity.h"
#include "GameWorld.h"

SPlayer::SPlayer(int id_, int pid_)
{
	id = id_;
	pid = pid_;
	
	Width = 56; 
	Height = 124; //test временно
	TFPack->ModelCharacter[id].setTextureRect(IntRect(0, 0, Width, Height));
	gamer = TFPack->ModelCharacter[id]; //загрузка модели
}

void SPlayer::update()
{
	gamer.setPosition(x, y);
	hb.setPosition(x - 20, y - 20);
	hb.setProgress((float)hp / (float)max_hp);
	hb.setHp(hp, max_hp);

	if (hp <= 0) life = false;
}

void SPlayer::updateHp(int atck)
{
	hp -= atck;
}
