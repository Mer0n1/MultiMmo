#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

/*
�������� ���������� ����������. ������� �����������
*/
//int algorithmCircle(Vector2f, float ratio); //ratio - ����������� (1 ��������)
/*
1 �������� ��������� ���������� �����
���������� ��������� �� ������ �� 20 ��������� � ����������� ��������� (32/32 - �������)
*/
Vector2f* algorithmLine(Vector2f begin, Vector2f end); 
/*
2 �������� ��������� ���������� �����
���������� ��������� �� ������ �� 20 ��������� � ����������� ��������� (32/32 - �� �������� �����)
�� ������� �����: �������� end.x - begin.x = ������� ����������. ���������� end �������, begin.x - �������������
*/
Vector2f* algorithmLine2Prototype(Vector2f begin, Vector2f end);
