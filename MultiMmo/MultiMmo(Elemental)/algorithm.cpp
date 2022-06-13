#include "algorithm.h"
#include <Windows.h>

Vector2f* algorithmLine(Vector2f begin, Vector2f end)
{
	//найдем направление
	Vector2f ItogMassv[20];
	Vector2f itog; //каждый тайловый итоговый ход

	int nx = 0, ny = 0; //направления
	if (begin.x < end.x) nx = 1; else nx = -1; //по графику
	if (begin.y < end.y) ny = 1; else ny = -1;

	float poX = begin.x - end.x; //сколько нужно тайлов пройти
	float poY = begin.y - end.y; //до нужной позиции

	for (int j = 0; j < 20; j++) { //x и y не должны превышать 1

		if (poX / poY > 1 || poX / poY < -1) { //если x превышает 1 то x = 1 а y < 1
			itog.x += nx;
			itog.y += poY / poX * nx;
		} else { //наоборот
			itog.x += poX / poY * ny;
			itog.y += ny;
		}
		Sleep(0); //из за бага 
		ItogMassv[j].x = begin.x + itog.x; //прорисовываем от конца до начала
		ItogMassv[j].y = begin.y + itog.y;
	}
	return &ItogMassv[0];
	//Отрезок как вектор, имеет направление (от begin до end)
}

Vector2f* algorithmLine2Prototype(Vector2f begin, Vector2f end)
{
	Vector2f Massv[20];
	float itogX = 0, itogY = 0; //каждый тайловый итоговый ход

	int nx = 0, ny = 0; //направления
	if (end.x + begin.x < end.x) nx = 1; else nx = -1;
	if (end.y + begin.y < end.y) ny = 1; else ny = -1;

	float poX = begin.x; //сколько нужно тайлов пройти
	float poY = begin.y; //до нужной позиции

	for (int j = 0; j < 20; j++)
	{
		if (poX / poY > 1 || poX / poY < -1) { //если x превышает 1 то x = 1 а y < 1

			itogX += nx;
			itogY += poY / poX * nx;
		}
		else { //наоборот
			itogX += poX / poY * ny;
			itogY += ny;
		}
		Massv[j].x = begin.x + end.x + itogX; // (прибавляем начало атаки к дополнению)
		Massv[j].y = begin.y + end.y + itogY;
	}
	return &Massv[0];
}