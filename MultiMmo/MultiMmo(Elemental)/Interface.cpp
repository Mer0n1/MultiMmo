#include "Interface.h"
#include "Windows.h"
#include <string>
#include <iostream>

HealthBarRpg::HealthBarRpg() : maxWidth(100)
{
    b.loadFromFile("Textures/interface/XPact_frame.png");
    b1.loadFromFile("Textures/interface/XPact_polos.png");
    b_mana.loadFromFile("Textures/interface/XPact_mana.png");

    q.setTexture(b); 
    q.setScale(1.3f, 1.3f);

    q1.setScale(1.3f, 1.3f); //0.00008
    q1.setTexture(b1); 
    q1.setPosition(340, 160);
    q1.setTextureRect(IntRect(0, 0, maxWidth, 9));

    mana.setTexture(b_mana); 
    mana.setScale(1.3f, 1.3f);

    font.loadFromFile("ttf/CyrilicOld.ttf");
    text.setFont(font);
    text.setPosition(q.getPosition().x + 5, q.getPosition().y + 2);
    text.setCharacterSize(12);
    text.setFillColor(Color::Black);
    text.setStyle(Text::Bold);
    text.setString("0/0");
}

void HealthBarRpg::saveSetScale(float w, float h)
{
    q.setScale(w, h); 
    q1.setScale(w, h);
    mana.setScale(w, h); 
}

void HealthBarRpg::setPosition(int x, int y) //задает координаты
{
    q.setPosition(x, y);
    q1.setPosition(x, y);
    mana.setPosition(x, y + 50);
}

void HealthBarRpg::setProgress(float p) // p в диапазоне [0, 1]
{
    int pr1 = static_cast<int>(p * maxWidth);
    if (pr1 < 0) pr1 = 0;
    if (pr1 > maxWidth) pr1 = maxWidth;
    q1.setTextureRect(IntRect(0, 0, pr1, 9));
}

void HealthBarRpg::setHp(int hp, int maxHp)
{
    text.setPosition(q.getPosition().x + 5, q.getPosition().y - 15);
    text.setString(to_string(hp) + "/" + to_string(maxHp));
}

void HealthBarRpg::draw(RenderWindow& window)
{
    window.draw(q1); window.draw(q);

    text.setPosition(q.getPosition().x + 5, q.getPosition().y - 15);
    window.draw(text);
}

/////////////////////////////////////////////////////////////////////////////////
//RechargeAttack

RechargeAttack::RechargeAttack()
{

}

RechargeAttack::RechargeAttack(int nAt, int recharge_) : recharge(recharge_)
{
    model_.loadFromFile("Textures/icon/time_ac.png"); 
    spr.setTexture(model_);
    spr.setTextureRect(IntRect(0, 0, 40, 40));
   
    if (nAt == 1) spr.setPosition(305, 475); 
    if (nAt == 2) spr.setPosition(355, 475);
    if (nAt == 3) spr.setPosition(435, 475);
    
    time = recharge + 1;
}

void RechargeAttack::StartTimer()
{
    if (time >= recharge) { //начало таймера только если он уже был истек ранее 
        clock.restart();
        time = 0;
    }
}

void RechargeAttack::update(RenderWindow &window)
{
    if (time > recharge) return;

    time = clock.getElapsedTime().asSeconds();

    float indx = 21.f / recharge;
    int numb = floor(indx * time);
    int x = 0, y = 0;
    int os = 0; //остаток

    if (numb > 6) {
        os = floor(numb / 7);
        numb = numb - os * 7;

        y = os;
        x = numb;
    }
    else
        x = numb;

    spr.setTextureRect(IntRect(x * 39, y * 39, 39, 39));
    spr.setTexture(model_);
    window.draw(spr);
}

void RechargeAttack::setPosition(int x, int y)
{
    spr.setPosition(x, y);
}

/////////////////////////////////////////////////////////////////////////////////

Reference::Reference()
{
    saveTab = false;

    referenceTab_.loadFromFile("Textures/interface/ReferenceTab.png");
    refMenu_.loadFromFile("Textures/interface/menuRef.png");

    referenceTab.setTexture(referenceTab_);
    referenceTab.setPosition(500, 5);

    refMenu.setTexture(refMenu_);
    refMenu.setPosition(300, 5);
}

void Reference::draw(RenderWindow& window)
{
    if (Keyboard::isKeyPressed(Keyboard::Tab))
    {
        if (!saveTab)
            saveTab = true;
        else
            saveTab = false;
        Sleep(200);
    }
    
    if (saveTab)
        window.draw(refMenu);
    else
        window.draw(referenceTab);
}

void Reference::setPositionView(int x, int y)
{
    referenceTab.setPosition(x+280, y-300);
    refMenu.setPosition(x-100, y - 290);
}

/////////////////////////////////////////////////////////////////////////////////
//RpgInterface

RpgInterface::RpgInterface(HealthBarRpg* hb_, RechargeAttack* rech_, View* view_)
{
    sleeptime = 0; 
    time = 0;

    frame_.loadFromFile("Textures/icon/frame.png");

    frame.setTexture(frame_);
    frame.setPosition(20, 500);

    for (int j = 0; j < 3; j++) {
        f[j].setTexture(frame_);
        f[j].setScale(0.8, 0.8);
    }
    f[0].setPosition(110, 270); 
    f[1].setPosition(190, 270); 
    f[2].setPosition(270, 270); 

    view = view_;
    hb = hb_;
    ref = new Reference;

    for (int j = 0; j < 3; j++) 
        rech[j] = rech_++; 
    
}

void RpgInterface::setPosition(int posx, int posy)
{ 
    for (int j = 0; j < 3; j++) {
        f[j].setPosition(posx + 80 * j - 130, posy + 170);
        rech[j]->setPosition(posx + 80 * j - 126, posy + 171);
    }
  
    frame.setPosition(posx - 370, posy + 200);
    hb->setPosition(view->getCenter().x - 300, view->getCenter().y + 200);
    ref->setPositionView(view->getCenter().x, view->getCenter().y);
}

void RpgInterface::Interface(RenderWindow& window) //вывод основного интерфейса
{
    Vector2i pixelPos = Mouse::getPosition(window); //забираем координаты мыши
    Vector2f pos = window.mapPixelToCoords(pixelPos); //переводим их в игровые
    time = clock.getElapsedTime().asSeconds();
   
    int menunum = 0;

    if (IntRect(20, 500, 60, 60).contains(Mouse::getPosition(window)))  
        menunum = 1;  //меню профиля

    if (Mouse::isButtonPressed(Mouse::Left) && time > sleeptime)
    {
        if (menunum == 1) {} //меню профиля

        sleeptime = time + 0.2; 
        Sleep(200);
    }

    //------------------Draw
    
    for (int j = 0; j < 3; j++) {
        window.draw(f[j]); //вывод рамок
        rech[j]->update(window); //вывод таймеров
    }

    window.draw(frame);
    //hb->draw(window); //вывод полоски хп (полоска хп выводится в GameWorld)
    ref->draw(window); //вывод справки
}

