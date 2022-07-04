#include "SubEntity.h"


Mobs::Mobs(string name_, int id_, int pid_) : Enemy(name_, id_, pid_)
{
	model_ = *gamer.getTexture();
	gamer.setTextureRect(IntRect(0, 0, Width, Height)); //текстура для анимации имеет другую IntRect

	if (id == 102) animation_.setAnimation(0); //test у 102 нет анимации

	delete AttackMode; //мобы не используют атаку
	AttackMode = NULL;
	time = 0;
}

Mobs::~Mobs()
{
}

void Mobs::update()
{
	time = clock.getElapsedTime().asSeconds();
	
	gamer.setPosition(x, y);
	hb.setPosition(x - 10, y - 10);
	hb.setProgress((float)hp / (float)max_hp);
	hb.setHp(hp, max_hp);
	
	if (id > 99) Control(); else controlEnemy();
	interactionWithMap(); 
	go_move(); 
	ai();
	
	animation_.animationContol(gamer, model_); //анимация движения
	if (hp <= 0) { life = false; gamer.setColor(Color::Black); }
}

void Mobs::updateHp(int atck)
{
	hp -= atck; 
}

void Mobs::Control()
{
	//движение
	if (forward) y -= speed * *TimeOptimization;
	if (right)   x += speed * *TimeOptimization;
	if (left)    x -= speed * *TimeOptimization;
	if ( back)   y += speed * *TimeOptimization;
}