#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using std::to_string;


class HealthBarRpg
{ 
    /* полоска хп */
private:
    const int maxWidth;

    Texture effect;
    Sprite effectf; // это для бафов или эффектов для полоски
    Texture b, b1, b_mana;
    Sprite q, q1, mana;

    Text text; //показатель хп
    Font font;
public:
    HealthBarRpg();
    void saveSetScale(float, float); //изменяет размер полоски хп
    void setPosition(int x, int y); //задает координаты
    void setProgress(float p); // p в диапазоне [0, 1] - обнеовить полоску
    void setXp(int xp, int maxXp); //установка хп
    void draw(RenderWindow& window);
};


class RechargeAttack
{
    /*
    Класс отвечающий за интерфейс показа перезарядки навыка
    */
public:
    RechargeAttack();
    RechargeAttack(int nAt, int recharge);

    void update(RenderWindow& window);
    void StartTimer();
    void setPosition(int x, int y);

 private:
    Sprite spr;
    Texture model_;

    float time;
    Clock clock;
    int recharge;
};

class Reference
{
    /* Справка-обучалка игре, показ клавиш (пока что вместо настроек)*/
    Texture referenceTab_, refMenu_;
    Sprite referenceTab, refMenu;
    bool saveTab;
public:
    Reference();
    void draw(RenderWindow& window);
    void setPositionView(int x, int y);
};

class RpgInterface
{
private:
    HealthBarRpg* hb; //полоска хп нашего аватара
    RechargeAttack* rech[3]; //3 таймера перезарядки атак
    Reference* ref; 
    View* view;

    Texture frame_;
    Sprite frame; //основная рамка
    Sprite f[3]; //3 рамки способностей

    int sleeptime;
    int time;
    Clock clock;

public:
    RpgInterface(HealthBarRpg* hb_, RechargeAttack* rech, View* view_);
    void setPosition(int posx, int posy); //задать позицию
    void Interface(RenderWindow& window); //вывод и обработка интерфейса
};
