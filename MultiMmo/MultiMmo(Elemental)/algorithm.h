#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

/*
Алгоритм построения окружности. Функция незавершена
*/
//int algorithmCircle(Vector2f, float ratio); //ratio - соотношение (1 стандарт)
/*
1 прототип алгоритма построение линии
Возвращает указатель на массив из 20 элементов с результатом координат (32/32 - мировых)
*/
Vector2f* algorithmLine(Vector2f begin, Vector2f end); 
/*
2 прототип алгоритма построения линии
Возвращает указатель на массив из 20 элементов с результатом координат (32/32 - от конечной точки)
От конечно точки: означает end.x - begin.x = мировые координаты. Координаты end мировые, begin.x - относительные
*/
Vector2f* algorithmLine2Prototype(Vector2f begin, Vector2f end);
