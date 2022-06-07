#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using std::to_string;


class HealthBarRpg
{ 
    /* ������� �� */
private:
    const int maxWidth;

    Texture effect;
    Sprite effectf; // ��� ��� ����� ��� �������� ��� �������
    Texture b, b1, b_mana;
    Sprite q, q1, mana;

    Text text; //���������� ��
    Font font;
public:
    HealthBarRpg();
    void saveSetScale(float, float); //�������� ������ ������� ��
    void setPosition(int x, int y); //������ ����������
    void setProgress(float p); // p � ��������� [0, 1] - ��������� �������
    void setXp(int xp, int maxXp); //��������� ��
    void draw(RenderWindow& window);
};


class RechargeAttack
{
    /*
    ����� ���������� �� ��������� ������ ����������� ������
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
    /* �������-�������� ����, ����� ������ (���� ��� ������ ��������)*/
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
    HealthBarRpg* hb; //������� �� ������ �������
    RechargeAttack* rech[3]; //3 ������� ����������� ����
    Reference* ref; 
    View* view;

    Texture frame_;
    Sprite frame; //�������� �����
    Sprite f[3]; //3 ����� ������������

    int sleeptime;
    int time;
    Clock clock;

public:
    RpgInterface(HealthBarRpg* hb_, RechargeAttack* rech, View* view_);
    void setPosition(int posx, int posy); //������ �������
    void Interface(RenderWindow& window); //����� � ��������� ����������
};
