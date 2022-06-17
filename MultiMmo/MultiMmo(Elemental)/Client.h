#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Tcp.h"
#include <fstream>
#include <thread>
#include "JATS.h"
#include <string>
#include "Avatar.h"
#include "Entity.h" 

using std::string;
using std::to_string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

struct siden //structure identefication
{
	string typeConnection;
	string password;
	string email;
	string username;
};

class Client
{
public:
	Client();
	~Client();
	static void InisializationWorld(GameWorld* world); //иницилизируем 1 раз

	void Damage(int p_id, int damage); //нанесение урона
	int start(siden ident); //запуск подключения к серверу
private:
	void sockReady(); //2 поток принятие запросов от сервера и отправка
	void sockReadyWorld(); //принятие и отправка udp запросов мира

	static GameWorld* world;
	Avatar avatar;
	Entity* MyPlayer;

	//Tcp
	WSAData wsaData_;
	SOCKADDR_IN addr_;
	SOCKET Connection_; //QTcpSocket - соединение с сервером
	string Descriptor; //номер сокета на сервере

	//udp
	SOCKET sock; //QUdp - сокет для пересылки структур данных
	sockaddr_in server_addr, client_addr;
	int server_len, client_len;

	//прочее
	JsonDocument doc;
	Clock clock;
	float time; // таймер
	float save_time;
};

class ClientMenu
{
	//меню авторизации/регистрации
	Texture interface_[3];
	Sprite interface_s[3];

	string password_s, login_s, username_s;

	Text password, login, email,
		username, signIn, signUp;
	Font font;

	Client* client;
public:
	ClientMenu(Client* client);
	void MenuAut(RenderWindow& window); //меню авторизации
	bool MenuReg(RenderWindow& window); //меню регистрации
	void LoadSave(); //загрузить сохраненные данные о паролях
};