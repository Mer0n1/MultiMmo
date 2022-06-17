#include "Client.h"
#include "GameWorld.h"

Client::Client()
{
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
	delete MyPlayer;
}

int Client::start(siden ident)
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
	
	//Самый 1 сегмент на подключение
	cout << "Connected!\n";

	char msg[10000];
	memset(msg, '\x0', 10000);

	read(Connection_, msg, sizeof(msg));

	doc = string(msg);
	Descriptor = doc.value("Descriptor").toString(); //вытащим дескриптор
	memset(msg, '\x0', 256); //clear
	
	//2 сегмент - запрос на идентефикацию
	//"{\"protocol\":\"identefication\",\"type\":\"aut\",\"gmail\":\"rd\",\"password\":\"123\"}" //пример запроса
	string len;
	if (ident.typeConnection == "aut")
		len = "{\"protocol\":\"identefication\",\"type\":\"aut\",\"gmail\":\"" + ident.email +
		"\",\"password\":\"" + ident.password + "\"}";
	else
		len = "{\"protocol\":\"identefication\",\"type\":\"reg\",\"gmail\":\"" + ident.email +
		"\",\"password\":\"" + ident.password + "\",\"name\":\"" + ident.username + "\"}";
	
	send(Connection_, len.c_str(), len.length(), NULL);
	read(Connection_, msg, sizeof(msg));
	Sleep(200);
	
	//3 сегмент - запрос на вход в мир (тестовой)
	len = "{\"protocol\":\"EnterTheWorld\"}"; //новый протокол входа в мир
	send(Connection_, len.c_str(), len.length(), NULL); //отправка сегмента на вход в мир

	thread th(&Client::sockReady, this);
	th.detach(); //Запуск 2 потока принимающего запросы
	
	doc = string(msg); //расшифровка json формата
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

		//Далее запросы
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
	{
		for (int j = 0; j != world->getSize(); j++) //обновляем характеристики этого персонажа
			if (world->getEntity(j)->getPid() == avatar.pid)
			{
				if (avatar.x < 0 || avatar.x > 15000) avatar.x = 0; //тест (ограничение если придет поврежденная структура)
				if (avatar.y < 0 || avatar.y > 15000) avatar.y = 0;
				
				world->setAvatar(&avatar, j);
 				break;
			}
			else
				if (j == world->getSize() - 1) { //если такого нет создаем обьект
					world->addEntity(new SPlayer(avatar.id, avatar.pid)); //добавляем сетевого игрока
					world->setAvatar(&avatar, j); //можно же вместо сета описать данные в конструкторе
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

/////////////////////////////////////////////////////////////////////////////////
ClientMenu::ClientMenu(Client* client_)
{
	interface_[0].loadFromFile("Textures/identefication/MenuM.png");
	interface_[1].loadFromFile("Textures/identefication/autorization.png");
	interface_[2].loadFromFile("Textures/identefication/registration.png");

	for (int j = 0; j < 3; j++)
		interface_s[j].setTexture(interface_[j]);

	interface_s[1].setPosition(242, 63);
	interface_s[2].setPosition(242, 63);


	font.loadFromFile("ttf/CyrilicOld.ttf");

	password.setFont(font);
	password.setPosition(260, 210);
	password.setCharacterSize(16);
	password.setFillColor(Color(177, 203, 226));
	password.setStyle(Text::Bold);
	password.setString("password");

	login = email = username = password;

	email.setPosition(260, 160);
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

	client = client_;
}

void ClientMenu::MenuAut(RenderWindow& window)
{
	LoadSave(); //загрузка данных
	string* universal = NULL;
	int true1 = true;

	while (true1)
	{
		int menunum = 0;
		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			if (events.type == Event::TextEntered)
				if (events.text.unicode < 128 & universal != NULL)
				{
					if (events.text.unicode != 8)
						*universal += static_cast<char>(events.text.unicode);
					else if (universal->length() != 0) universal->erase(universal->length() - 1);
				}
		}
		email.setFillColor(Color(177, 203, 226));
		password.setFillColor(Color(177, 203, 226));
		username.setFillColor(Color(177, 203, 226));
		signIn.setFillColor(Color::White);
		signUp.setFillColor(Color::White);
		if (universal == &login_s)    email.setFillColor(Color::White);
		if (universal == &password_s) password.setFillColor(Color::White);

		signIn.setPosition(365, 300);
		signIn.setCharacterSize(18);
		signIn.setString("Sign In");

		signUp.setPosition(320, 340);
		signUp.setCharacterSize(12);
		signUp.setString("Do you not account? Sign Up");
		email.setPosition(260, 160);

		if (IntRect(282, 277, 235, 60).contains(Mouse::getPosition(window)))
			menunum = 1; //button Sign In
		if (IntRect(320, 340, 190, 15).contains(Mouse::getPosition(window)))
			menunum = 2; //menu Sign Up
		if (IntRect(240, 160, 290, 40).contains(Mouse::getPosition(window))) {
			email.setFillColor(Color::White); menunum = 3;
		} //Email
		if (IntRect(260, 200, 290, 40).contains(Mouse::getPosition(window))) {
			password.setFillColor(Color::White); menunum = 4;
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menunum == 1) //авторизация
				if (password_s[0] != '\x0' &
					login_s[0] != '\x0') //строки ввода не должны быть пустыми
				{
					siden s;
					s.email = login_s;
					s.password = password_s;
					s.typeConnection = "aut";

					if (!client->start(s)) {
						ofstream infile; 
						infile.open("Save/authorization.txt");
						infile << login_s << ' ' << password_s << ' ';
						break;
					}
					Sleep(300);
				}

			if (menunum == 2) 
				if (MenuReg(window)) break; //выход если регистрация успешна
			if (menunum == 3) {  //фиксируем клик по емайл-строке
				universal = &login_s;
				email.setFillColor(Color::White);
			}
			if (menunum == 4) { //фиксируем клик по пароль-строке
				universal = &password_s;
				password.setFillColor(Color::White);
			}
		}

		if (password_s[0] == '\x0' && universal != &password_s) password_s = "Password";
		if (login_s[0] == '\x0' && universal != &login_s) login_s = "Gmail";
		if (login_s == "Gmail" && universal == &login_s)  login_s = "";
		if (password_s == "Password" & universal == &password_s) password_s = "";

		email.setString(login_s);
		password.setString(password_s);

		//---Draw
		window.draw(interface_s[0]);
		window.draw(interface_s[1]);

		//Text
		window.draw(email);
		window.draw(password);
		window.draw(signIn);
		window.draw(signUp);

		window.display();
	}
}

bool ClientMenu::MenuReg(RenderWindow& window)
{
	string* universal = NULL;

	while (true)
	{
		int menunum = 0;
		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			if (events.type == Event::TextEntered)
				if (events.text.unicode < 128 && universal != NULL)
				{
					if (events.text.unicode != 8)
						*universal += static_cast<char>(events.text.unicode);
					else if (universal->length() != 0) universal->erase(universal->length() - 1);
				}
		}
		email.setFillColor(Color(177, 203, 226));
		password.setFillColor(Color(177, 203, 226));
		username.setFillColor(Color(177, 203, 226));
		signIn.setFillColor(Color::White);
		signUp.setFillColor(Color::White);

		if (universal == &username_s)  username.setFillColor(Color::White);
		if (universal == &login_s)     email.setFillColor(Color::White);
		if (universal == &password_s) password.setFillColor(Color::White);

		signIn.setPosition(375, 380);
		signIn.setCharacterSize(12);
		signIn.setString("Sign In");
		signUp.setPosition(365, 330);
		signUp.setCharacterSize(18);
		signUp.setString("Sign Up");
		email.setPosition(260, 260);

		if (IntRect(283, 325, 210, 50).contains(Mouse::getPosition(window)))
			menunum = 1; //кнопка регистрировать
		if (IntRect(365, 380, 55, 15).contains(Mouse::getPosition(window)))
			menunum = 2; //войти в меню sign in
		if (IntRect(260, 150, 290, 40).contains(Mouse::getPosition(window))) {
			username.setFillColor(Color::White); menunum = 3;
		}
		if (IntRect(240, 200, 290, 40).contains(Mouse::getPosition(window))) {
			password.setFillColor(Color::White); menunum = 4;
		} //log
		if (IntRect(240, 260, 290, 40).contains(Mouse::getPosition(window))) {
			email.setFillColor(Color::White); menunum = 5;
		} //Email

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menunum == 1) //авторизация
				if (password_s[0] != '\x0' &
					username_s[0] != '\x0' &
					login_s[0] != '\x0')
				{
					siden s;
					s.email = login_s;
					s.password = password_s;
					s.typeConnection = "reg";
					s.username = username_s;

					if (!client->start(s)) return true;
					Sleep(300);
				}

			if (menunum == 2) { return false; }
			if (menunum == 3) {
				universal = &username_s;
				username.setFillColor(Color::White);
			}
			if (menunum == 4) {
				universal = &password_s;
				password.setFillColor(Color::White);
			}
			if (menunum == 5) {
				universal = &login_s;
				email.setFillColor(Color::White);
			}
		}

		if (username_s[0] == '\x0' && universal != &username_s) { username_s = "Username"; }
		if (password_s[0] == '\x0' && universal != &password_s) { password_s = "Password"; }
		if (login_s[0] == '\x0' && universal != &login_s) { login_s = "Gmail"; }

		if (login_s == "Gmail" && universal == &login_s) login_s = "";
		if (username_s == "Username" && universal == &username_s) username_s = "";
		if (password_s == "Password" && universal == &password_s) password_s = "";

		username.setString(username_s);
		password.setString(password_s);
		email.setString(login_s);

		//Draw
		window.draw(interface_s[0]);
		window.draw(interface_s[2]);

		window.draw(email);
		window.draw(password);
		window.draw(signIn);
		window.draw(signUp);
		window.draw(username);

		window.display();
	}
}

void ClientMenu::LoadSave()
{
	ifstream outfile;
	ofstream infile;
	string pas1, log1;

	outfile.open("Save/authorization.txt");
	outfile >> log1 >> pas1; //сохраненные пароль и логин

	if (log1 != "\x0" || pas1 != "\x0")
	{
		password_s = pas1;
		login_s = log1;
	}
	password.setString(password_s);
	email.setString(login_s);
}