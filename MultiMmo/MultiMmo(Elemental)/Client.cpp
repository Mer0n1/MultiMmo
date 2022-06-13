#include "Client.h"
#include "GameWorld.h"

Client::Client()
{
	//Графика
	font.loadFromFile("ttf/CyrilicOld.ttf");

	password.setFont(font);
	password.setPosition(260, 210);
	password.setCharacterSize(16);
	password.setFillColor(Color(177, 203, 226));
	password.setStyle(Text::Bold);
	password.setString("password");

	login = email = username = password;

	email.setPosition(260, 160); //260
	email.setString("Email");

	username.setPosition(260, 160);
	username.setString("Username");

	signIn = signUp = password;

	signIn.setPosition(375, 380);
	signIn.setCharacterSize(12);
	signIn.setFillColor(Color::White);
	signIn.setString("Sign In");

	signUp.setPosition(365, 330);
	signUp.setCharacterSize(18);
	signUp.setFillColor(Color::White);
	signUp.setString("Sign Up");

	time = 0;
	save_time = 0;

	//udp соединение
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(MAKEWORD(2, 2), &data);

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int OptVal = 1;
	int OptLen = sizeof(int);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&OptVal, OptLen);

	server_len = sizeof(server_addr);
	ZeroMemory(&server_addr, server_len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5556);
	inet_pton(AF_INET, "176.196.135.47", &server_addr.sin_addr); //176.196.135.47   127.0.0.1

	client_len = sizeof(client_addr);
	ZeroMemory(&client_addr, client_len);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(0); //порт для отправки от сервера (было 0)
	client_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	::bind(sock, (struct sockaddr*)&client_addr, client_len);
	
}

Client::~Client()
{
	delete gamer;
	delete MyPlayer;
}

void Client::ClientMenu(RenderWindow& window)
{
	//-----Загрузка текстур
	Texture interface_[3];
	interface_[0].loadFromFile("Textures/identefication/MenuM.png");
	interface_[1].loadFromFile("Textures/identefication/autorization.png");
	interface_[2].loadFromFile("Textures/identefication/registration.png");
	Sprite interface_s[3];
	for (int j = 0; j < 3; j++)
		interface_s[j].setTexture(interface_[j]);
	interface_s[1].setPosition(242, 63);
	interface_s[2].setPosition(242, 63);
	//-------------------------------------

	ifstream outfile;
	outfile.open("Save/authorization.txt");
	ofstream infile;

	string pas1, log1; 
	outfile >> log1 >> pas1; //сохраненные пароль и логин
	if (log1 == "\x0" || pas1 == "\x0")
		cout << "\nПустые данных сохраненных паролей\n";
	else {
		password_s = pas1;
		login_s = log1;
	}

	string uk = "avc";
	string* universal = &uk;
	string nool;
	
	int cennum = 0; //фиксированный выбор
	int ident = 1; //тип идентефикации (1 - авторизация, 0 - регистрация)
	int true1 = true;

	while (true1)
	{
		int menunum = 0;
		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			else if (events.type == Event::MouseWheelMoved);
		}
		email.setFillColor(Color(177, 203, 226));
		password.setFillColor(Color(177, 203, 226));
		username.setFillColor(Color(177, 203, 226));

		if (ident == 1) {
			if (IntRect(282, 277, 235, 60).contains(Mouse::getPosition(window))) { menunum = 1; } //Sign In
			if (IntRect(320, 340, 190, 15).contains(Mouse::getPosition(window))) { menunum = 2; } //Sign Up
			if (IntRect(240, 160, 290, 40).contains(Mouse::getPosition(window))) { email.setFillColor(Color::White); menunum = 3; } //Email
		}
		else {
			if (IntRect(240, 260, 290, 40).contains(Mouse::getPosition(window))) { email.setFillColor(Color::White); menunum = 3; } //Email
			if (IntRect(260, 150, 290, 40).contains(Mouse::getPosition(window))) { username.setFillColor(Color::White); menunum = 4; }
			if (IntRect(283, 325, 210, 50).contains(Mouse::getPosition(window))) { menunum = 7; }
			if (IntRect(365, 380, 55, 15).contains(Mouse::getPosition(window))) { menunum = 6; }
		}
		if (IntRect(260, 200, 290, 40).contains(Mouse::getPosition(window))) {
			password.setFillColor(Color::White); menunum = 5;
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menunum == 2) { ident = 0; }
			if (menunum == 6) { ident = 1; }

			if (menunum == 4) { cennum = 1; }
			if (menunum == 5) { cennum = 2; }
			if (menunum == 3) { cennum = 3; }

			if (menunum == 1) { //авторизация
				if (password_s[0] != '\x0' & login_s[0] != '\x0')
				{
					typeConnection = "aut";
					true1 = start(); Sleep(300);
				}
			}
			if (menunum == 7) { //регистрация
				if (password_s[0] != '\x0' & username_s[0] != '\x0' & login_s[0] != '\x0')
				{
					typeConnection = "reg";
					true1 = start(); Sleep(300);
				}
			}
		}

		if (ident == 1) { //окно авторизации
			signIn.setPosition(365, 300);
			signIn.setCharacterSize(18);
			signIn.setFillColor(Color::White);
			signIn.setString("Sign In");

			signUp.setPosition(320, 340);
			signUp.setCharacterSize(12);
			signUp.setFillColor(Color::White);
			signUp.setString("Do you not account? Sign Up");

			email.setPosition(260, 160);
		}
		else {
			signIn.setPosition(375, 380);
			signIn.setCharacterSize(12);
			signIn.setFillColor(Color::White);
			signIn.setString("Sign In");

			signUp.setPosition(365, 330);
			signUp.setCharacterSize(18);
			signUp.setFillColor(Color::White);
			signUp.setString("Sign Up");

			email.setPosition(260, 260);
		}
		if (cennum == 1) {
			username.setString("|");
			universal = &username_s;
			username.setFillColor(Color::White);
		}
		else username.setString("Username");
		if (cennum == 2) {
			password.setString("|");
			universal = &password_s;
			password.setFillColor(Color::White);
		}
		else password.setString("password");
		if (cennum == 3) {
			email.setString("|");
			universal = &login_s;
			email.setFillColor(Color::White);
		}
		else email.setString("Email");
		if (cennum != 0) {
			if (username_s[0] != '\x0') username.setString(username_s);
			if (password_s[0] != '\x0') password.setString(password_s);
			if (login_s[0] != '\x0') email.setString(login_s);
		}

		if (events.type == Event::TextEntered) {
			if (Keyboard::isKeyPressed(Keyboard::Q)) { *universal += "q"; }
			if (Keyboard::isKeyPressed(Keyboard::W)) { *universal += "w"; }
			if (Keyboard::isKeyPressed(Keyboard::E)) { *universal += "e"; }
			if (Keyboard::isKeyPressed(Keyboard::R)) { *universal += "r"; } 
			if (Keyboard::isKeyPressed(Keyboard::T)) { *universal += "t"; }
			if (Keyboard::isKeyPressed(Keyboard::Y)) { *universal += "y"; }
			if (Keyboard::isKeyPressed(Keyboard::U)) { *universal += "u"; }
			if (Keyboard::isKeyPressed(Keyboard::I)) { *universal += "i"; }
			if (Keyboard::isKeyPressed(Keyboard::O)) { *universal += "o"; }
			if (Keyboard::isKeyPressed(Keyboard::P)) { *universal += "p"; }
			if (Keyboard::isKeyPressed(Keyboard::A)) { *universal += "a"; }
			if (Keyboard::isKeyPressed(Keyboard::S)) { *universal += "s"; }
			if (Keyboard::isKeyPressed(Keyboard::D)) { *universal += "d"; }
			if (Keyboard::isKeyPressed(Keyboard::F)) { *universal += "f"; }
			if (Keyboard::isKeyPressed(Keyboard::G)) { *universal += "g"; }
			if (Keyboard::isKeyPressed(Keyboard::H)) { *universal += "h"; }
			if (Keyboard::isKeyPressed(Keyboard::J)) { *universal += "j"; }
			if (Keyboard::isKeyPressed(Keyboard::K)) { *universal += "k"; }
			if (Keyboard::isKeyPressed(Keyboard::L)) { *universal += "l"; }
			if (Keyboard::isKeyPressed(Keyboard::Z)) { *universal += "z"; }
			if (Keyboard::isKeyPressed(Keyboard::X)) { *universal += "x"; }
			if (Keyboard::isKeyPressed(Keyboard::C)) { *universal += "c"; }
			if (Keyboard::isKeyPressed(Keyboard::V)) { *universal += "v"; }
			if (Keyboard::isKeyPressed(Keyboard::B)) { *universal += "b"; }
			if (Keyboard::isKeyPressed(Keyboard::N)) { *universal += "n"; }
			if (Keyboard::isKeyPressed(Keyboard::M)) { *universal += "m"; }

			if (Keyboard::isKeyPressed(Keyboard::Num0)) { *universal += "0"; }
			if (Keyboard::isKeyPressed(Keyboard::Num1)) { *universal += "1"; }
			if (Keyboard::isKeyPressed(Keyboard::Num2)) { *universal += "2"; }
			if (Keyboard::isKeyPressed(Keyboard::Num3)) { *universal += "3"; }
			if (Keyboard::isKeyPressed(Keyboard::Num4)) { *universal += "4"; }
			if (Keyboard::isKeyPressed(Keyboard::Num5)) { *universal += "5"; }
			if (Keyboard::isKeyPressed(Keyboard::Num6)) { *universal += "6"; }
			if (Keyboard::isKeyPressed(Keyboard::Num7)) { *universal += "7"; }
			if (Keyboard::isKeyPressed(Keyboard::Num8)) { *universal += "8"; }
			if (Keyboard::isKeyPressed(Keyboard::Num9)) { *universal += "9"; }


			if (Keyboard::isKeyPressed(Keyboard::BackSpace) & *universal[0].c_str() != '\x0') {
				string zam = *universal;
				for (int j = 0; j < zam.length() + 1; j++) {
					if (zam[j] == '\x0') { zam.erase(j - 1, j); *universal = zam; break; }
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::Num2)) { *universal += "@"; }
			if (Keyboard::isKeyPressed(Keyboard::Period) && *universal != nool) {
				string zam = *universal;
				for (int j = 1; j < zam.length() + 1; j++) {
					if (zam[j] == '\x0' & zam[j - 1] != '.')
					{
						zam += "."; *universal = zam; break;
					}
				}
			}

			//Comma - запятая
			Sleep(200);
		}

		//Draw
		window.draw(interface_s[0]);
		if (ident == 1) window.draw(interface_s[1]);
		else            window.draw(interface_s[2]);

		//Text
		if (ident != 1) window.draw(username);
		window.draw(email); window.draw(password);
		window.draw(signIn); window.draw(signUp);

		window.display();
	}
	infile.open("Save/authorization.txt");
	infile << login_s << ' ' << password_s << ' ';
}

int Client::start()
{
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData_) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	int sizeofaddr = sizeof(addr_);

	addr_.sin_addr.s_addr = inet_addr("176.196.135.47"); 
	addr_.sin_port = htons(5555);
	addr_.sin_family = AF_INET;

	while (true) {
		Connection_ = socket(AF_INET, SOCK_STREAM, NULL);

		if (connect(Connection_, (SOCKADDR*)&addr_, sizeof(addr_)) != 0)
			cout << "Error: failed connect to server.\n";
		else break;
	}

	cout << "Connected!\n";
	
	char msg[10000];
	for (int j = 0; j < 10000; j++)
		msg[j] = '\x0';

	read(Connection_, msg, sizeof(msg));

	for (int j = 34; j < 100; j++)
		if (msg[j] != ' ')
			Descriptor += msg[j]; //вытащим дескриптор

	for (int j = 0; j < 256; j++)
		msg[j] = ' ';  //очистить

	//1 сегмент - запрос на авторизацию
	//"{\"protocol\":\"identefication\",\"type\":\"aut\",\"gmail\":\"rd\",\"password\":\"123\"}" //пример запроса
	string len;
	if (typeConnection == "aut")
		len = "{\"protocol\":\"identefication\",\"type\":\"aut\",\"gmail\":\"" + login_s +
		"\",\"password\":\"" + password_s + "\"}";
	else
		len = "{\"protocol\":\"identefication\",\"type\":\"reg\",\"gmail\":\"" + login_s +
		"\",\"password\":\"" + password_s + "\",\"name\":\"" + username_s + "\"}";

	send(Connection_, len.c_str(), len.length(), NULL);
	read(Connection_, msg, sizeof(msg));
	Sleep(200);

	len = "{\"protocol\":\"EnterTheWorld\"}"; //новый протокол входа в мир
	send(Connection_, len.c_str(), len.length(), NULL); //отправка сегмента на вход в мир

	thread th(&Client::sockReady, this);
	th.detach(); //Запуск 2 потока принимающего запросы

	string msg_ = msg;
	doc = msg_; //расшифровка json формата
	
	if (doc.value("Write").toString() == "User auntification" ||
		doc.value("Write").toString() == "Registation thinks!") {
		return false;
	}
	return true; 
}


void Client::sockReady()
{
	//Принять пакет данных емайл
	int rc = 0; //проверка состояние запросов и сокетов
	char packet[10000]; //массив для приема tcp запросов
	string data;

	//делаем сокеты неблокирующими
	u_long mode = 1; //неблокирующий сокет (для цикла, чтобы не было приостановок) (0 - отключение)
	ioctlsocket(Connection_, FIONBIO, &mode);

	u_long mode_Udp = 1;
	ioctlsocket(sock, FIONBIO, &mode_Udp);


	while (true) //tcp и udp запросы ( udp постоянно отправляются)
	{
		sockReadyWorld(); //udp запросы мира
		
		//if (read(Connection_, packet, sizeof(packet)) < 0) continue; //принимаем запрос
		if (recv(Connection_, packet, sizeof(packet), NULL) < 0) continue; //принимаем запрос

		data = packet;
		doc = data; //установка на json

		///Далее запросы
		if (doc.value("type").toString() == "Unconnect")
			for (int j = 0; j < world->getSize(); j++)
				if (world->getEntity(j)->getPid() == atoi(doc.value("p_id").toString().c_str()))
					world->deleteEntity(world->getEntity(j)->getId(), world->getEntity(j)->getPid()); //удаляем обьект игрока который вышел
		
		if (doc.value("type").toString() == "kick") {
			cout << "\nСервер посчитал вас ботом и кикнул...\n\n";
			exit(0); //запрос кика
		}
	}
}

void Client::sockReadyWorld()
{
	
	time = clock.getElapsedTime().asSeconds();
	MyPlayer = world->getEntity(0);
	
	avatar.socketDescriptor = atoi(Descriptor.c_str());
	avatar.id = MyPlayer->getId();
	avatar.life = MyPlayer->getLife();
	avatar.maxHp = MyPlayer->getMaxHp();
	avatar.pid = MyPlayer->getPid();
	avatar.hp = MyPlayer->getHp();
	avatar.x = MyPlayer->getPos().x;
	avatar.y = MyPlayer->getPos().y; 

	if (time > save_time) { //отправка структуры раз в 0.05 сек, а проверка бесконечная
		sendto(sock, reinterpret_cast<char*>(&avatar), sizeof(avatar), 0, (sockaddr*)&server_addr, server_len);
		save_time = time + 0.05; 
	}
	
	Sleep(10); //тест
	
	if (recvfrom(sock, reinterpret_cast<char*>(&avatar), 256, 0, (sockaddr*)&server_addr, &server_len) > 0) //принимаем
	{ //с gamer что то не так - сокет выдает -1
		//добавлении обьекта Entity и его характеристик
		gamer = &avatar;

		for (int j = 0; j != world->getSize(); j++)
			if (world->getEntity(j)->getPid() == gamer->pid)
			{
				if (gamer->x < -1000 || gamer->x > 5000) gamer->x = 0;
				if (gamer->y < -1000 || gamer->y > 5000) gamer->y = 0;
				
				world->setAvatar(gamer, j); 
 				break;
			}
			else
				if (j == world->getSize() - 1) { //если такого нет создаем обьект
					world->addEntity(new SPlayer(gamer->id, gamer->pid)); //добавляем сетевого игрока
					world->setAvatar(gamer, j); //можно же вместо сета описать данные в конструкторе
				}
	}
	
}

void Client::Damage(int p_id, int damage)
{
	string data = "{\"type\":\"Attack\",\"p_id\":\"" + to_string(p_id) + "\",\"damage\":\"" + to_string(damage) + "\"}";
	sendto(sock, data.c_str(), data.length(), 0, (sockaddr*)&server_addr, server_len);
}

void Client::InisializationWorld(GameWorld* world_)
{
	world = world_; 
}

GameWorld* Client::world = NULL;

