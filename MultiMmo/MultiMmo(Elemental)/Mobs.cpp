#include "SubEntity.h"


Mobs::Mobs(string name_, int id_, int pid_) : Enemy(name_, id_, pid_)
{
	model_ = *gamer.getTexture();
	gamer.setTextureRect(IntRect(0, 0, Width, Height)); //�������� ��� �������� ����� ������ IntRect
	
	time = 0;
	saveTime = 0;
	CurCadr = 1;

	if (id == 102) animation_.setAnimation(0); //test � 102 ��� ��������
}

Mobs::~Mobs()
{
}

void Mobs::update()
{
	time = clock.getElapsedTime().asSeconds();

	gamer.setPosition(x, y);
	hb.setPosition(x - 10, y - 10);
	hb.setProgress((float)xp / (float)max_Xp);
	hb.setXp(xp, max_Xp);
	
	if (id > 99) Control(); else controlEnemy();
	interactionWithMap(); 
	go_move(); 
	ai();
	
	animation_.animationContol(gamer, model_); //�������� ��������
	if (xp <= 0) { life = false; gamer.setColor(Color::Black); }
}

void Mobs::updateXp(int atck)
{
	xp -= atck;
}

void Mobs::Control()
{
	if (time < controltime) return;
	controltime = time + 0.01;

	//��������
	if (forward) y -= speed;
	if (right)   x += speed;
	if (left)    x -= speed;
	if ( back)   y += speed;
}