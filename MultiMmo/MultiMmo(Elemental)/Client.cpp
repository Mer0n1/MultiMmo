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
	inet_pton(AF_INET, "176.196.135.47", &server_addr.sin_addr); 

	client_len = sizeof(client_addr);
	ZeroMemory(&client_addr, client_len);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(0); //порт для отправки от сервера (было 0)
	client_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	::bind(sock, (struct sockaddr*)&client_addr, client_len);
	
}

Client::~Client()
{
}

int Client::start(siden ident)
{
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData_) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	int sizeofaddr = sizeof(addr_);

	addr_.sin_addr.s_addr = inet_addr("176.196.135.47"); //176.196.135.47
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
	char packet[1024]; //массив для приема tcp запросов
	string data;
	
	//делаем сокеты неблокирующими
	u_long mode = 1; //неблокирующий сокет (для цикла, чтобы не было приостановок) (0 - отключение)
	ioctlsocket(Connection_, FIONBIO, &mode);

	u_long mode_Udp = 1;
	ioctlsocket(sock, FIONBIO, &mode_Udp);


	while (true) //tcp и udp запросы ( udp постоянно отправляются)
	{
		if (!ActiveMap.empty())
			sockReadyWorld(); //udp запросы мира
		
		rc = recv(Connection_, packet, sizeof(packet), NULL);
		if (rc < 0) continue; //принимаем запрос
		if (rc == 0) exit(0); //выход при потере соединения с сервером
		if (rc == 1024) 
			while (rc > 0)
			{
				data.append(packet);
				memset(packet, 0, 1024);
				rc = recv(Connection_, packet, sizeof(packet)-1, NULL);
			}
		else data = packet;
		
		doc = data;

		//Далее запросы
		if (doc.value("Protocol").toString() == "Unconnect")
			for (int j = 0; j < world->getSize(); j++)
				if (world->getEntity(j)->getPid() == atoi(doc.value("p_id").toString().c_str()))
					world->deleteEntity(world->getEntity(j)->getId(), world->getEntity(j)->getPid()); //удаляем обьект игрока который вышел
		
		if (doc.value("Protocol").toString() == "kick") {
			cout << "\nСервер посчитал вас ботом и кикнул...\n\n";
			exit(0); //запрос кика
		}
		if (doc.value("Protocol").toString() == "LoadServMap") { //протокол загрузки карты
			TiXmlDocument object("maps/TestServerMap/Life.xml"); //+name от doc.value(nameMap);
			object.LoadFile(); 
			//Разделим слипнувшиеся запросы
			vector<string> DataArr;
			string current;
			for (int j = 0; j < data.length(); j++)
			{
				if (data[j] == '\n') {
					DataArr.push_back(current);
					current.clear();
					continue;
				}
				current += data[j];
			}
			for (int j = 0; j < DataArr.size(); j++)
			{
				doc = DataArr[j];
				//Protocol: LoadMap, type: begin, map: TestServerMap, ... //Возможно ptype: AddEntity/AddGroup/
				if (doc.value("ptype").toString() == "begin") {
					object.Clear(); //очищает
					TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "utf-8");
					auto* World = new TiXmlElement("World");
					object.LinkEndChild(decl);
					object.LinkEndChild(World);
				}

				auto* character = new TiXmlElement("character");
				object.FirstChildElement()->LinkEndChild(character);
				character->SetAttribute("type", doc.value("type").toString().c_str());
				character->SetAttribute("id", doc.value("id").toString().c_str());
				character->SetAttribute("IDO", "0");
				character->SetAttribute("name", doc.value("name").toString().c_str());
				character->SetAttribute("x", doc.value("x").toString().c_str());
				character->SetAttribute("y", doc.value("y").toString().c_str());
				character->SetAttribute("hp", doc.value("hp").toString().c_str());
				character->SetAttribute("maxHp", doc.value("maxHp").toString().c_str());
				character->SetAttribute("speed", doc.value("speed").toString().c_str());
				character->SetAttribute("visibility", doc.value("visibility").toString().c_str());
				character->SetAttribute("DistanceFromOpponent", doc.value("DistanceFromOpponent").toString().c_str());
				character->SetAttribute("DistantionPoint", doc.value("DistantionPoint").toString().c_str());
				object.SaveFile("maps/TestServerMap/Life.xml");
			}
			//Отправка удачного подтверждения о загрузке карты
			string len = "{\"protocol\":\"EnterTheWorld\",\"name\":\"" + ActiveMap + "\",\"type\":\"successful\"}"; 
			send(Connection_, len.c_str(), len.length(), NULL); 
		}
		if (doc.value("Protocol").toString() == "SC")
			statusCS = true;
		
		memset(packet, 0, sizeof(packet));
		data.clear();
	}
}
void Client::sockReadyWorld()
{
	time = clock.getElapsedTime().asSeconds();

	if (time > save_time)
	{
		for (int j = 0; j < world->getSize(); j++)
		{
			avatar.socketDescriptor = atoi(Descriptor.c_str());
			if (statusCS) {
				if (typeid(*world->getEntity(j)).name() == static_cast<string>("class SPlayer"))
					continue; //по сути любом аватар добавленный от сервера - игрок сервера
				if (typeid(*world->getEntity(j)).name() != static_cast<string>("class Player"))
				    avatar.socketDescriptor = 0;  //мобы имеют 0 дескриптор
			}
			
			avatar.hp =    world->getEntity(j)->getHp();
			avatar.id =    world->getEntity(j)->getId();
			avatar.maxHp = world->getEntity(j)->getMaxHp();
			avatar.x =     world->getEntity(j)->getPos().x;
			avatar.y =     world->getEntity(j)->getPos().y;
			avatar.pid =   world->getEntity(j)->getPid();
			avatar.life =  world->getEntity(j)->getLife();
			
			sendto(sock, reinterpret_cast<char*>(&avatar), sizeof(avatar), 0, (sockaddr*)&server_addr, server_len);
			if (!statusCS) break; //j=0 - это игрок. Без статуса мобы не отправляются
			Sleep(1); 
		}
		save_time = time + 0.05;
	}

	Sleep(10); //тест
	
	while (true) //принимаем до тех пор пока есть очередь/загруженность пакетов
		if (recvfrom(sock, udpData, 1024, 0, (sockaddr*)&server_addr, &server_len) > 0) //принимаем
		{
			if (((Avatar*)(&udpData))->id < 200 && ((Avatar*)(&udpData))->id > 0) {
				avatar = *((Avatar*)(&udpData));
				//if (avatar.pid == world->getEntity(0)->getPid()) break;  //проверка на собственное принятие
				
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
			else ReadyAttack(); //если это json то читаем
		}
		else break;

}

void Client::ReadyAttack()
{
	doc = (string)udpData; 
	int j = doc.value("number").toInt();
	int pid = doc.value("pid").toInt();
	int quan = doc.value("quan").toInt();
	int size = doc.value("size").toInt();
	string type = doc.value("type").toString();
	string identn = doc.value("identn").toString();

	if (type == "delete")
	{
		for (int j = 0; j < aserv.vec.size(); j++)
			if (aserv.vec[j].pid == pid && aserv.vec[j].identn == identn) {
				AnimationAttackModule::getObject().DeleteAnimation(aserv.vec[j].a);
				aserv.vec.erase(aserv.vec.begin() + j);
				break;
			}
	}

	if (type == "active" || type == "delay")
		for (int n = 0, k = -1; n < aserv.vec.size(); n++) {
			if (aserv.vec[n].pid == pid && aserv.vec[n].identn == identn)
				k = 0;
			else if (n == aserv.vec.size() - 1)
			{
				aserv.add(AnimationAttackModule::getObject().newScructureAnimation(), pid, identn);
				k = 1;
			}
			if (k > -1) {
				for (int j = 0; j < quan; j++)
				{
					aserv.vec[n + k].a->intTile_8[j].setPosition(doc.value("x" + to_string(j)).toInt(), doc.value("y" + to_string(j)).toInt());
					aserv.vec[n + k].a->intTile_8[j].setScale(size / 8.f, size / 8.f);
					aserv.vec[n + k].a->intTile_8[j].setColor(Color::Blue);
					aserv.vec[n + k].a->isActive = true;
					if (type == "delay") aserv.vec[n + k].a->intTile_8[j].setColor(Color(255, 255, 255, 50));
				}
				break;
			}
		}
	if (aserv.vec.size() == 0 && type != "delete") {
		aserv.add(AnimationAttackModule::getObject().newScructureAnimation(), pid, identn);

		for (int j = 0; j < quan; j++)
		{
			aserv.vec.back().a->intTile_8[j].setPosition(doc.value("x" + to_string(j)).toInt(), doc.value("y" + to_string(j)).toInt());
			aserv.vec.back().a->intTile_8[j].setScale(size / 8.f, size / 8.f);
			aserv.vec.back().a->intTile_8[j].setColor(Color::Blue);
			aserv.vec.back().a->isActive = true;
			if (type == "delay") aserv.vec.back().a->intTile_8[j].setColor(Color(255, 255, 255, 50));
		}
	}
}

void Client::Damage(int p_id, int damage)
{
	string data = "{\"Protocol\":\"Attack\",\"p_id\":\"" + to_string(p_id) + "\",\"damage\":\"" + to_string(damage) + "\"}";
	sendto(sock, data.c_str(), data.length(), 0, (sockaddr*)&server_addr, server_len);
}

void Client::ShowAttack(vector<Tile>& tiles, int quan, string type, string identn, int pid)
{
	string data; 
	data = "{\"Protocol\":\"ShowAttack\",\"size\":\"" + to_string(tiles[0].size) + "\",\"quan\":\"" + 
		to_string(quan) + "\",\"type\":\"" + type.c_str() + "\",\"pid\":\"" + to_string(pid) + "\",\"identn\":\"" + identn;

	for (int j = 0; j < quan; j++)
		if (tiles[j].coord.x != 0 && tiles[j].coord.y != 0)
			data += "\",\"x" + to_string(j) + "\":\"" + to_string(tiles[j].coord.x) +
			        "\",\"y" + to_string(j) + "\":\"" + to_string(tiles[j].coord.y);
		else break;
	data += "\"}";

	if(type == "delete") 
		data = "{\"Protocol\":\"ShowAttack\",\"type\":\"delete\",\"pid\":\"" + to_string(pid) + "\",\"identn\":\"" + identn + "\"}";

	sendto(sock, data.c_str(), data.length(), 0, (sockaddr*)&server_addr, server_len);
}


void Client::InisializationWorld(GameWorld* world_)
{
	world = world_; 
}

void Client::EnterWorld(string name, string type)
{
	//type: Exit (выход из мира) 
	if (ActiveMap.empty()) 
		ActiveMap = name;

	if (type == "Exit") //протокол выхода из мира
		ActiveMap.clear();

	string len = "{\"protocol\":\"EnterTheWorld\",\"name\":\"" + name + "\",\"type\":\"" + type + "\"}"; //новый протокол входа в мир
	send(Connection_, len.c_str(), len.length(), NULL); //отправка сегмента на вход в мир
	Sleep(3000); //ожидание загрузки карты (пока не придет Sc)
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