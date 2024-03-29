﻿#include <SFML/Graphics.hpp>
#include <iostream>

#include "Inventor.h"
#include "JATS.h"
#include "Profile.h"
#include "Client.h"
#include "BridgeClient.h"
using namespace sf;

int main()
{
	RenderWindow window(VideoMode(800, 600), "MultiMmo");       //создание окна
	Client client;
	ClientMenu menu(&client);
	Profile profile;
	BridgeClient::inisialization(&client); 

	srand(time(0));
	setlocale(LC_ALL, "Russian");
	window.setFramerateLimit(120);

	menu.MenuAut(window);
	profile.menu(window, client);
}