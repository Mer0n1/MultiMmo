#include "algorithm.h"
#include <Windows.h>

Vector2f* algorithmLine(Vector2f begin, Vector2f end)
{
	//������ �����������
	Vector2f ItogMassv[20];
	Vector2f itog; //������ �������� �������� ���

	int nx = 0, ny = 0; //�����������
	if (begin.x < end.x) nx = 1; else nx = -1; //�� �������
	if (begin.y < end.y) ny = 1; else ny = -1;

	float poX = begin.x - end.x; //������� ����� ������ ������
	float poY = begin.y - end.y; //�� ������ �������

	for (int j = 0; j < 20; j++) { //x � y �� ������ ��������� 1

		if (poX / poY > 1 || poX / poY < -1) { //���� x ��������� 1 �� x = 1 � y < 1
			itog.x += nx;
			itog.y += poY / poX * nx;
		} else { //��������
			itog.x += poX / poY * ny;
			itog.y += ny;
		}
		Sleep(0); //�� �� ���� 
		ItogMassv[j].x = begin.x + itog.x; //������������� �� ����� �� ������
		ItogMassv[j].y = begin.y + itog.y;
	}
	return &ItogMassv[0];
	//������� ��� ������, ����� ����������� (�� begin �� end)
}

Vector2f* algorithmLine2Prototype(Vector2f begin, Vector2f end)
{
	Vector2f Massv[20];
	float itogX = 0, itogY = 0; //������ �������� �������� ���

	int nx = 0, ny = 0; //�����������
	if (end.x + begin.x < end.x) nx = 1; else nx = -1;
	if (end.y + begin.y < end.y) ny = 1; else ny = -1;

	float poX = begin.x; //������� ����� ������ ������
	float poY = begin.y; //�� ������ �������

	for (int j = 0; j < 20; j++)
	{
		if (poX / poY > 1 || poX / poY < -1) { //���� x ��������� 1 �� x = 1 � y < 1

			itogX += nx;
			itogY += poY / poX * nx;
		}
		else { //��������
			itogX += poX / poY * ny;
			itogY += ny;
		}
		Massv[j].x = begin.x + end.x + itogX; // (���������� ������ ����� � ����������)
		Massv[j].y = begin.y + end.y + itogY;
	}
	return &Massv[0];
}