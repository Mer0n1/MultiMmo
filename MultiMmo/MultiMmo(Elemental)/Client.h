#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Tcp.h"
#include <fstream>
#include <thread>
#include "JATS.h"
#include <string>
#include "Avatar.h"
#include "Entity.h" //���� ������� ��� �������� GameWorld

using std::string;
using std::to_string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

class Client
{
public:
	Client();
	~Client();
	static void InisializationWorld(GameWorld* world); //������������� 1 ���

	void Damage(int p_id, int damage); //��������� �����
	void ClientMenu(RenderWindow& window); //���� �������������
	int start(); //������ ����������� � �������
private:
	void sockReady(); //2 ����� �������� �������� �� ������� � ��������
	void sockReadyWorld(); //�������� � �������� udp �������� ����

	static GameWorld* world;
	Avatar avatar;
	Avatar gamer; //������ ���������� ��� ������ ������ (�� ����������)
	Entity* MyPlayer;

	//Tcp
	WSAData wsaData_;
	SOCKADDR_IN addr_;
	SOCKET Connection_; //QTcpSocket - ���������� � ��������
	string Descriptor; //����� ������ �� �������

	//udp
	SOCKET sock; //QUdp - ����� ��� ��������� �������� ������
	sockaddr_in server_addr, client_addr;
	int server_len, client_len;

	//menu
	Text password, login, email,
		username, signIn, signUp;
	Font font;

	string typeConnection;
	string password_s, login_s, username_s;
	string sockReady_;

	//������
	JsonDocument doc;
	Clock clock;
	float time; // ������
	float save_time;
};
