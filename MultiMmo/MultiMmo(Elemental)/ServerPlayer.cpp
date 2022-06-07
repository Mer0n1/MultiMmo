#include "SubEntity.h"
#include "GameWorld.h"

SPlayer::SPlayer(int id_, int pid_)
{
	id = id_;
	pid = pid_;

	TFPack->ModelCharacter[id].setTextureRect(IntRect(Width * 2 + 45, 0, Width, Height));
	gamer = TFPack->ModelCharacter[95]; //загрузка модели
}

void SPlayer::update()
{
	gamer.setPosition(x, y);
	hb.setPosition(x - 20, y - 20);
	hb.setProgress((float)xp / (float)max_Xp);
	hb.setXp(xp, max_Xp);
}

void SPlayer::updateXp(int atck)
{

}
