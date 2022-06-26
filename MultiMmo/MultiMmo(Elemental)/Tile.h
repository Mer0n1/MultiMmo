#pragma once
#include <SFML/Graphics.hpp>

struct Tile
{
	sf::Vector2i coord;
	float delay, duration;
	int size;
};