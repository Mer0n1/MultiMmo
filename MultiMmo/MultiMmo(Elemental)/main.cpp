#include <SFML/Graphics.hpp>
#include <iostream>

#include "Inventor.h"
#include "JATS.h"
#include "Profile.h"
#include "Client.h"
#include "BridgeClient.h"
using namespace sf;


int main()
{
	RenderWindow window(VideoMode(800, 600), "MultiMmo");       //создание окна //800-600
	
	srand(time(0)); setlocale(LC_ALL, "Russian");
	window.setFramerateLimit(120);

	Client client;
	Profile profile;

	//client.ClientMenu(window);
	BridgeClient::inisialization(&client); //возможно можно было бы инициализировать этот класс в классе клиента
	
	profile.menu(window);
}