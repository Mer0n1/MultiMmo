#include "Redactor.h"
#include "Windows.h"

Redactor::Redactor()
{
	//��������
	newtex[0].loadFromFile("Textures/Redactor/TestFon.png");
	newtex[1].loadFromFile("Textures/Redactor/change.png");
	newtex[2].loadFromFile("Textures/profile/button.png");
	newtex[3].loadFromFile("Textures/models/Developer.png");

	interface_1.setTexture(newtex[0]);
	Create.setTexture(newtex[2]);
	Load.setTexture(newtex[2]);
	player.setTexture(newtex[3]);

	//�������
	change = new Sprite[10000];

	for (int j = 0; j < 10000; j++)
		change[j].setTexture(newtex[1]);

	distance = 160; 
	width = 70; 
	height = 150; 

	int TileToWidth = (distance * 2 + width) / 32; //��� �� ������ �� ������ ����� 

	for (int y = 0; y < (distance * 2 + height) / 32; y++)
		for (int x = 0; x < (distance * 2 + width) / 32; x++) 
			change[x + y * TileToWidth].setPosition(x * 32, y * 32);

	Create.setPosition(20, 350); 
	Create.setScale(0.8, 1);

	Load.setPosition(20, 300); 
	Load.setScale(0.8, 1);

	player.setPosition(160, 160); 
	player.setTextureRect(IntRect(0, 0, 55, 123));

	//�����
	font.loadFromFile("ttf/CyrilicOld.ttf");
	for (int j = 0; j < 35; j++) {
		text[j].setFont(font);
		text[j].setPosition(0, 0);
		text[j].setCharacterSize(18);
		text[j].setFillColor(Color::Black);
		text[j].setStyle(Text::Bold);
		text[j].setString(" ");
	}

	text[0].setString("������ 32/32 - 500 �"); 
	text[1].setString("��������� (�� �������� �� ���������) - 1500 � �� 1 ������"); 
	text[2].setString("��� �����:"); 
	text[3].setString("Do (damage once)"); 
	text[4].setString("dpsa (Damage Per Second)"); 
	text[5].setString("����/���");
	text[6].setString("����/��������� (0.1�~)"); 
	text[7].setString(OCH + "�����"); 
	text[8].setString("[2000]"); 
	text[9].setString("[5000]");
	text[10].setString("���������"); 
	text[11].setString("���������� ������: ");
	text[12].setString("�������� �����:  \n\n����������������� �����: ");
	text[13].setString("�������"); 
	text[13].setFillColor(Color::White); 
	text[14].setString("���������"); 
	text[14].setFillColor(Color::White); 
	text[15].setString("������� �����: "); 
	text[16].setString("�������������"); 
	text[17].setString("�� ����� �����"); 
	text[18].setString("�� �����������");
	text[19].setString("������ ������:    " + to_string(SizeTile));

	//�������
	text[0].setPosition(20, 60);
	text[1].setPosition(20, 90);
	text[2].setPosition(20, 120);
	text[3].setPosition(20, 140);
	text[4].setPosition(250, 140);
	text[5].setPosition(200, 160);
	text[6].setPosition(310, 160);
	text[7].setPosition(300, 50);
	text[8].setPosition(210, 180);
	text[9].setPosition(320, 180);
	text[10].setPosition(30, 160);
	text[11].setPosition(300, 290);
	text[12].setPosition(300, 230);
	text[13].setPosition(50, 350);
	text[14].setPosition(40, 300);
	text[15].setPosition(10, 200);
	text[16].setPosition(60, 220);
	text[17].setPosition(60, 240);
	text[18].setPosition(60, 260);
	text[19].setPosition(300, 200);

	text[7].setCharacterSize(25);
	text[8].setCharacterSize(13);
	text[9].setCharacterSize(13);
	text[10].setCharacterSize(13);
	text[13].setCharacterSize(22);
	text[14].setCharacterSize(22);
	text[19].setCharacterSize(18);


	//Arrows ��� ��������� 
	for (int j = 0; j < 10; j++) {
		arrows[j] = text[0];
		if (j % 2 > 0) 
			arrows[j].setString("<");
		else 
			arrows[j].setString(">");
		arrows[j].setPosition(-500, -500);
	}

	arrows[1].setPosition(420, 230); arrows[2].setPosition(460, 230); //delay �����
	arrows[3].setPosition(447, 262); arrows[4].setPosition(487, 262); //duration �����
	arrows[5].setPosition(440, 200); arrows[6].setPosition(480, 200); //������������ �������� ������

	//������������� ������ ����������
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 100; j++)
			coordinate[j][i] = 0;

	redCo[0] = 0, 
	redCo[1] = 0;
	view.reset(FloatRect(0, 0, 500, 500));
	memset(delay_, 0, 100 * 4);
	memset(duration_, 0, 100 * 4);

	width = 80; height = 150;
	OCH = 100000; //test 
	QuanTile = 20;
	distance = 160; //5 ������
	recharge = 1;
	SizeTile = 32;
	type_attack = "Do";
	FuncAttack = "Fixed";
	numC = 0;
	_Price = 0;
	_PriceTypeAttack = 0;
	wind = true; 
}


void Redactor::menu(RenderWindow& window)
{
	window.create(VideoMode(550, 400), "Redactor");
	redactor.create(VideoMode(500, 500), "Redactor");

	ostringstream delayS, durationS; 

	while (true)
	{
		int menunum = 0;
		wind = true;

		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			else if (events.type == Event::MouseWheelMoved);

			if (events.type == sf::Event::LostFocus)
				window.hasFocus();
		}
		//-------------------------------����������� ������
		for (int j = 0; j < 35; j++)
			text[j].setFillColor(Color::Black);
		for (int j = 0; j < 10; j++)
			arrows[j].setFillColor(Color::Black);

		if (FuncAttack == "Fixed") text[16].setFillColor(Color::White); 
		if (FuncAttack == "ClickPlace") text[17].setFillColor(Color::White);
		if (FuncAttack == "Direction") text[18].setFillColor(Color::White);

		if (type_attack == "Do") text[3].setFillColor(Color::White);
		if (type_attack == "dpsa") text[5].setFillColor(Color::White);
		if (type_attack == "ida") text[6].setFillColor(Color::White);
		
		text[13].setFillColor(Color::White); //������ �������
		text[14].setFillColor(Color::White); //������ ���������
		//-------------------------------
		
		if (IntRect(20, 500, 100, 30).contains(Mouse::getPosition(window))) 
			menunum = 1;
		if (IntRect(20, 60, 200, 20).contains(Mouse::getPosition(window))) {
			text[0].setFillColor(Color(200, 200, 200)); menunum = 2;
		} //������ ������
		if (IntRect(20, 90, 500, 20).contains(Mouse::getPosition(window))) {
			text[1].setFillColor(Color(200, 200, 200)); menunum = 3;
		} //������ ���������
		if (IntRect(20, 140, 130, 20).contains(Mouse::getPosition(window))) {
			text[3].setFillColor(Color(200, 200, 200)); menunum = 4;
		} //���������� ��� ����� Do
		if (IntRect(200, 160, 80, 20).contains(Mouse::getPosition(window))) {
			text[5].setFillColor(Color(200, 200, 200)); menunum = 5;
		} //��� ����� Ur/sec
		if (IntRect(310, 160, 200, 20).contains(Mouse::getPosition(window))) {
			text[6].setFillColor(Color(200, 200, 200)); menunum = 6;
		} //��� ����� Ur/mgn
		if (IntRect(140, 485, 100, 20).contains(Mouse::getPosition(window))) {
			text[12].setFillColor(Color(200, 200, 200)); menunum = 7;
		} //��������� �����������
		if (IntRect(arrows[2].getPosition().x, arrows[2].getPosition().y, 20, 20).contains(Mouse::getPosition(window))) { 
			arrows[2].setFillColor(Color(200, 200, 200)); menunum = 8; //��� ������� �� ������� delay ������
		}
		if (IntRect(arrows[1].getPosition().x, arrows[1].getPosition().y, 20, 20).contains(Mouse::getPosition(window))) { 
			arrows[1].setFillColor(Color(200, 200, 200)); menunum = 9; //delay ����������
		}
		if (IntRect(arrows[4].getPosition().x, arrows[4].getPosition().y, 20, 20).contains(Mouse::getPosition(window))) { 
			arrows[4].setFillColor(Color(200, 200, 200)); menunum = 10; //���������� duration 
		}
		if (IntRect(arrows[3].getPosition().x, arrows[3].getPosition().y, 20, 20).contains(Mouse::getPosition(window))) { 
			arrows[3].setFillColor(Color(200, 200, 200)); menunum = 11; //���������� duration 
		}
		if (IntRect(60, 220, 140, 20).contains(Mouse::getPosition(window))) {
			text[16].setFillColor(Color::White); menunum = 12;
		} //������������� �������
		if (IntRect(60, 240, 140, 20).contains(Mouse::getPosition(window))) {
			text[17].setFillColor(Color::White); menunum = 13;
		} //������� �� ����� �����
		if (IntRect(60, 260, 140, 20).contains(Mouse::getPosition(window))) {
			text[18].setFillColor(Color::White); menunum = 14;
		} //������� �� �����������
		if (IntRect(arrows[5].getPosition().x, arrows[5].getPosition().y, 20, 20).contains(Mouse::getPosition(window))) { 
			arrows[5].setFillColor(Color(200, 200, 200)); menunum = 15; //��������� ������ ������
		}
		if (IntRect(arrows[6].getPosition().x, arrows[6].getPosition().y, 20, 20).contains(Mouse::getPosition(window))) { 
			arrows[6].setFillColor(Color(200, 200, 200)); menunum = 16; //��������� ������ ������
		}
		
		if (IntRect(Create.getPosition().x, Create.getPosition().y, 140, 35).contains(Mouse::getPosition(window))) {
			menunum = 17;
		} //������� �����
		if (IntRect(Load.getPosition().x, Load.getPosition().y, 140, 35).contains(Mouse::getPosition(window))) {
			menunum = 18;
		} //��������� �����

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.create(VideoMode(800, 600), "Redactor"); //���������� ������� �����
			redactor.close(); //��������� 2 �����
			return; //�����
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menunum == 1) {

			}
			if (menunum == 2 && OCH >= 500) {
				OCH -= 500; QuanTile++;
				Sleep(200);
			}
			if (menunum == 3 && OCH >= 1500) {

				OCH -= 1500; 
				distance += 32;

				for (int y = 0; y < (distance * 2 + height) / SizeTile; y++)
					for (int x = 0; x < (distance * 2 + width) / SizeTile; x++) {
						change[x + y * ((distance * 2 + width) / SizeTile)].setPosition(x * SizeTile, y * SizeTile);
						change[x + y * ((distance * 2 + width) / SizeTile)].setScale(SizeTile / 32.f, SizeTile / 32.f);
					}

				player.setPosition(160, 160); //����� ������� 
				player.move(distance - 160, distance - 160); //����������� ������ ������������ ����������� ���������

				Sleep(200);
			}
			if (menunum == 4) {
				type_attack = "Do";
				_PriceTypeAttack = 0;
			}
			if (menunum == 5 && OCH - _Price >= 2000 && type_attack != "dpsa") { //������� ��������� && ���������� ����� && ��� ����� �� ����� ��
				_PriceTypeAttack = 2000;
				type_attack = "dpsa"; //dpsa (Ur/sec)
				Sleep(200);
			}
			if (menunum == 6 && OCH - _Price >= 5000 && type_attack != "ida") {
				_PriceTypeAttack = 5000;
				type_attack = "ida"; //dpsa (Ur/mgn)
				Sleep(200);
			}
			if (menunum == 7 && OCH - _Price >= 500) {
				_Price += 500;
				recharge -= 1;
				Sleep(200);
			}
			
			//----------������� delay � duration ������
			if (menunum == 8)
					if (redCo[0] != 0 && redCo[1] != 0) { //������� ��������� �����
						delay_[numC] += 0.1;
						_Price += 50;
						Sleep(200);
					}

			if (menunum == 9)
				if (delay_[numC] >= 0.1) //������� ����������� ��������
					if (redCo[0] != 0 && redCo[1] != 0) { //������� ��������� �����
						delay_[numC] -= 0.1;
						_Price -= 50;
						Sleep(200);
					}

			if (menunum == 10)
					if (redCo[0] != 0 && redCo[1] != 0) {
						duration_[numC] += 0.1;
						_Price += 50;
						Sleep(200);
					}

			if (menunum == 11)
				if (duration_[numC] >= 0.1)
					if (redCo[0] != 0 && redCo[1] != 0) {
						duration_[numC] -= 0.1;
						_Price -= 50;
						Sleep(200);
					}
			//-------------------------
			//�������� ������ ������
			if (menunum == 15)
			{
				if (SizeTile == 8) SizeTile = 4;
				if (SizeTile == 16) SizeTile = 8;
				if (SizeTile == 24) SizeTile = 16;
				if (SizeTile == 32) SizeTile = 24;
				if (SizeTile == 64) SizeTile = 32;

				for (int j = 0; j < 10000; j++)
					change[j].setPosition(-100, -100);
				
				int TileToWidth = (distance * 2 + width) / SizeTile;
				
				for (int y = 0; y < (distance * 2 + height) / SizeTile; y++)
					for (int x = 0; x < (distance * 2 + width) / SizeTile; x++) {
						change[x + y * TileToWidth].setPosition(x* SizeTile, y* SizeTile);
						change[x + y * TileToWidth].setScale(SizeTile / 32.f, SizeTile / 32.f);
					}
				
				newX = 0;
				newY = 0; //����� ���������
				player.setPosition(160 + distance-160, 160 + distance - 160); //����� �������
				Sleep(200);
			}

			if (menunum == 16)
			{
				if (SizeTile == 32) SizeTile = 64;
				if (SizeTile == 24) SizeTile = 32;
				if (SizeTile == 16) SizeTile = 24;
				if (SizeTile == 8) SizeTile = 16;
				if (SizeTile == 4) SizeTile = 8;

				for (int j = 0; j < 10000; j++)
					change[j].setPosition(-100, -100);

				int TileToWidth = (distance * 2 + width) / SizeTile;

				for (int y = 0; y < (distance * 2 + height) / SizeTile; y++)
					for (int x = 0; x < (distance * 2 + width) / SizeTile; x++) {
						change[x + y * TileToWidth].setScale(SizeTile / 32.f, SizeTile / 32.f);
						change[x + y * TileToWidth].setPosition(x * SizeTile, y * SizeTile);
					}

				newX = 0;
				newY = 0; //����� ���������
				player.setPosition(160 + distance - 160, 160 + distance - 160); //����� �������
				Sleep(200);
			}
			//-----------------------

			if (menunum == 12) FuncAttack = "Fixed"; //3 ������� �����
			if (menunum == 13) FuncAttack = "ClickPlace";
			if (menunum == 14) FuncAttack = "Direction";

			if (menunum == 17) 
				if (OCH - _Price - _PriceTypeAttack >= 0) 
					CreateAttack();
			//if (menunum == 18) loading(8, "Click"); //��������� ������������ �����

		}

		//----------------------------------------------------------------------
		delayS.str(""); 
		delayS << delay_[numC]; //�������� �����
		durationS.str(""); 
		durationS << duration_[numC]; //����������������� �����

		text[7].setString(to_string(OCH - _Price - _PriceTypeAttack) + " �����");
		text[11].setString("���������� ������: " + to_string(QuanTile));
		text[12].setString("Delay �����:     " + delayS.str() + "\n\nDuration �����:     " + durationS.str());
		text[19].setString("������ ������:    " + to_string(SizeTile));
		//------------------------------------------------------------------------

		window.clear();

		world(redactor);
		window.draw(interface_1);
		window.draw(Create); //������ �������
		window.draw(Load); //������ ��������� ����������

		for (int j = 0; j < 35; j++) //����� ������
			window.draw(text[j]);
		for (int j = 0; j < 10; j++) //����� ���������
			window.draw(arrows[j]);

		window.display();
	}
}

void Redactor::world(RenderWindow& redactor)
{
	int TileToWidth = ((distance * 2 + width) / SizeTile); //��� �� ������ �� x � y �����
	
	Vector2i pixelPos = Mouse::getPosition(redactor);
	Vector2f pos = redactor.mapPixelToCoords(pixelPos);
	pos.x -= newX;
	pos.y -= newY; //������������� ������� ������� ��� ����������� ������
	
	Event events;
	while (redactor.pollEvent(events))
	{
		if (events.type == Event::Closed)
			redactor.close();
		else if (events.type == Event::MouseWheelMoved);

		if (events.type == sf::Event::LostFocus)
			redactor.hasFocus();
	}
	
	//---------����������� ������
	for (int j = 0; j < 10000; j++)
		change[j].setColor(Color::White);

	for (int j = 0; j < 100; j++) 
			change[coordinate[j][0] + coordinate[j][1] * TileToWidth].setColor(Color::Blue);
	
	change[redCo[0] + redCo[1] * TileToWidth].setColor(Color::Red);
	//-------------------------------------

	if (!IntRect(player.getPosition().x - distance, player.getPosition().y - distance,
		2 * distance + width - 14, 2 * distance + height - 20).contains(Mouse::getPosition(redactor)))
		wind = false; //��������� ���� �� �������� ���������� ���������

	if (Mouse::isButtonPressed(Mouse::Left) && wind)
	{
		for (int j = 0; j < QuanTile; j++) { //�������� ���������

			if (coordinate[j][0] == (int)(pos.x / SizeTile) && //������� ��������� (�����)
				coordinate[j][1] == (int)(pos.y / SizeTile)) {
				
				coordinate[j][0] = 0;
				coordinate[j][1] = 0; 

				for (int i = j; i < QuanTile; i++) { //�����							
					coordinate[i][0] = coordinate[i + 1][0];
					coordinate[i][1] = coordinate[i + 1][1];
				}
				break;
			}
			else
				if (coordinate[j][0] == 0 && coordinate[j][1] == 0) { //��������
					coordinate[j][0] = pos.x / SizeTile;
					coordinate[j][1] = pos.y / SizeTile;
					break;
				}
		}
		Sleep(200);
	}

	//��������� � ���������� ����������� ����� �������������
	if (Mouse::isButtonPressed(Mouse::Right))
	{
		//�������� �������� ��������� - ��� ��������� �������� � ������������
		for (int j = 0; j < QuanTile; j++)
			if (coordinate[j][0] == (int)(pos.x / SizeTile) && //����� -12 ��� ������������� ��� setPosition
				coordinate[j][1] == (int)(pos.y / SizeTile)) {
				redCo[0] = coordinate[j][0];
				redCo[1] = coordinate[j][1];
				numC = j;
				break;
			}
		
		Sleep(100);
	}

	//------------------------------------------
	//���������� ������� ������
	if (Keyboard::isKeyPressed(Keyboard::Up)) { SpriteMove(0, 5); newY += 5; } //0 1
	if (Keyboard::isKeyPressed(Keyboard::Left)) { SpriteMove(5, 0); newX += 5; }
	if (Keyboard::isKeyPressed(Keyboard::Right)) { SpriteMove(-5, 0); newX += -5; }
	if (Keyboard::isKeyPressed(Keyboard::Down)) { SpriteMove(0, -5); newY += -5; }

	//---------------------------- //������� ���������
	RectangleShape Ysh(Vector2f(1.f, 2 * distance + height - 20));
	RectangleShape Ysh2(Vector2f(2.f, 2 * distance + height - 20));
	RectangleShape Xsh(Vector2f(2 * distance + width - 14, 2.f));
	RectangleShape Xsh2(Vector2f(2 * distance + width - 17, 2.f));
	CoorDistantion(Ysh, Ysh2, Xsh, Xsh2);
	//---------------------------

	redactor.clear();
	redactor.setView(view);

	for (int j = 0; j < 10000; j++) //����� ������ (625)
		redactor.draw(change[j]);

	redactor.draw(Ysh); redactor.draw(Xsh); //����� ������ ���������
	redactor.draw(Ysh2); redactor.draw(Xsh2);

	redactor.draw(player); //����� ���������
	redactor.display();

}

void Redactor::SpriteMove(int x, int y)
{
	for (int j = 0; j < 10000; j++)
		change[j].move(x, y);
	player.move(x, y);
}

void Redactor::CoorDistantion(RectangleShape& Ysh, RectangleShape& Ysh2, RectangleShape& Xsh, RectangleShape& Xsh2)
{
	Ysh.setFillColor(Color::Red);
	Ysh2.setFillColor(Color::Red);
	Xsh.setFillColor(Color::Red);
	Xsh2.setFillColor(Color::Red);
	Ysh.setPosition(player.getPosition().x - distance, player.getPosition().y - distance);
	Ysh2.setPosition(player.getPosition().x + distance + width - 14, player.getPosition().y + distance + height - 20);
	Xsh.setPosition(player.getPosition().x - distance, player.getPosition().y - distance);
	Xsh2.setPosition(player.getPosition().x + distance + width - 16, player.getPosition().y + distance + height - 20);
	Ysh2.setRotation(180); Xsh2.setRotation(180);
}

void Redactor::CreateAttack()
{
	int CurrentTile[100][2];
	int TileToWidth = (distance * 2 + width) / SizeTile; //��� �� ������ �� x � y �����

	if (SizeTile == 64) 
		player.setPosition(128, 128); //������������ ��� � ����������� ������
	
	for (int j = 0; j < QuanTile; j++)
		for (int c = 0; c < 2; c++)
			CurrentTile[j][c] = coordinate[j][c];

	for (int j = 0; j < QuanTile; j++) //��������� � ���������� ������������ ������
		if (!(CurrentTile[j][0] == 0 && CurrentTile[j][1] == 0)) {

			int x1 = rint(change[CurrentTile[j][0] + CurrentTile[j][1] * TileToWidth].getPosition().x / SizeTile - player.getPosition().x / SizeTile);
			int y1 = rint(change[CurrentTile[j][0] + CurrentTile[j][1] * TileToWidth].getPosition().y / SizeTile - player.getPosition().y / SizeTile);
			CurrentTile[j][0] = x1;
			CurrentTile[j][1] = y1;
		}

	if (SizeTile == 64)
		player.setPosition(160, 160);

	//xml
	TiXmlDocument object("Save/account.xml");
	if (!object.LoadFile()) {
		cout << "Loading level \"" << "inventor.xml" << "\" failed.-" << endl; 
		return;
	}

	TiXmlElement* sizeQ = object.FirstChildElement("account")->FirstChildElement("inventor");

	//������ ��������� SA ������� � ��� ����
	TiXmlElement* SAElement;
	SAElement = sizeQ->FirstChildElement("SA");
	int EndId = 0; //�������� ���� (��������� ����� �����)

	for (int j = 0; j < 45; j++) {
		string cp = SAElement->Attribute("id");
		EndId = atoi(cp.c_str());
		if (!(SAElement = SAElement->NextSiblingElement("SA")))
			break; //���� ��������� ������ ��� �������
	}
	//����� ������� ����� SA
	auto* newSA = new TiXmlElement("SA");
	sizeQ->LinkEndChild(newSA);
	newSA->SetDoubleAttribute("id", EndId + 1);
	newSA->SetAttribute("name", "Test");

	//� ��������� ��������������
	auto* delayTix = new TiXmlElement("delay");
	newSA->LinkEndChild(delayTix);

	ostringstream delayStr;
	for (int j = 0; j < QuanTile; j++) {
		delayStr << delay_[j]; 
		delayStr << ' ';
	}
	delayTix->SetAttribute("Value", delayStr.str().c_str());

	auto* durationTix = new TiXmlElement("duration");
	newSA->LinkEndChild(durationTix);
	ostringstream durationStr;
	for (int j = 0; j < QuanTile; j++) {
		durationStr << duration_[j]; 
		durationStr << ' ';
	}
	durationTix->SetAttribute("Value", durationStr.str().c_str());


	auto* TileTix = new TiXmlElement("Tile");
	newSA->LinkEndChild(TileTix);
	string TileStrX, TileStrY;
	for (int c = 0; c < QuanTile; c++) {
		TileStrX += to_string(CurrentTile[c][0]) + " ";
		TileStrY += to_string(CurrentTile[c][1]) + " ";
	}
	TileTix->SetAttribute("ValueX", TileStrX.c_str());
	TileTix->SetAttribute("ValueY", TileStrY.c_str());


	auto* additional = new TiXmlElement("Description");
	newSA->LinkEndChild(additional);

	additional->SetAttribute("FuncAttack", FuncAttack.c_str());
	additional->SetAttribute("TypeAttack", type_attack.c_str());
	additional->SetAttribute("Distantion", distance / SizeTile);
	additional->SetAttribute("Recharge", recharge);
	additional->SetAttribute("QuanityTile", QuanTile);
	additional->SetAttribute("Size", SizeTile);

	object.SaveFile();
	Sleep(300);
}


