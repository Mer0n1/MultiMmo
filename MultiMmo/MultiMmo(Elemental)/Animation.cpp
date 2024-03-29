#include "Animation.h"

AnimationAttackModule::AnimationAttackModule()
{
	change8.loadFromFile("Textures/Redactor/Tile_8.png"); 

	change.setTexture(changeT);
	change_8.setTexture(change8);
	change_8.setColor(Color::Blue);
}

AnimationAttackModule::~AnimationAttackModule()
{
	for (int j = 0; j < list.size(); j++) delete list[j];
}

AnimationAttack* AnimationAttackModule::newScructureAnimation()
{
	AnimationAttack* nw = new AnimationAttack();
	//nw->window = window;
	for (int j = 0; j < 100; j++) 
		nw->intTile_8[j] = change_8;
	
	list.push_back(nw);
	return nw;
}

void AnimationAttackModule::DeleteAnimation(AnimationAttack* animation_)
{
	for (int j = 0; j < list.size(); j++)
		if (list[j] == animation_)
		{
			list.erase(list.begin() + j);
			delete animation_;
			break; 
		}
}

void AnimationAttackModule::draw(RenderWindow& window)
{
	for (int j = 0; j < list.size(); j++)
		if (list[j]->isActive)
			for (int c = 0; c < sizeof(list[j]->intTile_8) / sizeof(list[j]->intTile_8[0]); c++) 
				window.draw(list[j]->intTile_8[c]);
}

void AnimationAttackModule::setWindow(RenderWindow& window_)
{
	object.window = &window_;
}

AnimationAttackModule& AnimationAttackModule::getObject()
{
	return object;
}

AnimationAttackModule AnimationAttackModule::object;

//-------------------------------AnimationMotion

AnimationMotionModule::AnimationMotionModule()
{

}

//--------------------------------Animation

Animation::Animation()
{
	time = 0;
	saveTime = 0;
	isAnimation = true;
}

void Animation::animationContol(Sprite& model, Texture& model_)
{
	if (!isAnimation) return;

	int width = 112, height = 100;
	time = clock.getElapsedTime().asSeconds();

	int QW = model_.getSize().x / 112;
	int QH = model_.getSize().y / 100;

	int CurW = (width * CurCadr) / 112;
	int CurH = 0;

	for (int j = 0; j < QH; j++)
		if (CurW >= QW) { CurW -= QW; CurH++; }
	if (CurCadr > QW * QH - 1) CurCadr = 1;

	if (saveTime < time) {
		model.setTextureRect(IntRect(CurW * width, CurH * height, width, height));

		CurCadr++;
		saveTime = time + 1 / 15.f;
	}
}

void Animation::setAnimation(bool o)
{
	if (o)
		isAnimation = true;
	else
		isAnimation = false;
}
