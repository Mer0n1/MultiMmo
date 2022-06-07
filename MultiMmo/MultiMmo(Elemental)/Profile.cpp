#include "Profile.h"
#include "Windows.h"

Profile::Profile() 
{
	texture[0].loadFromFile("Textures/profile/MenuProfile.png");
	texture[1].loadFromFile("Textures/profile/character_fon.jpg"); //��� ���������
	texture[2].loadFromFile("Textures/profile/button.png");
	texture[3] = texture[2];
	texture[4].loadFromFile("Textures/profile/characterInv.png");
	texture[5].loadFromFile("Textures/profile/abilityInv.png");
	texture[6].loadFromFile("Textures/profile/frameAbility.png");
	texture[8] = texture[7] = texture[6]; //3 ������ �����������
	texture[9].loadFromFile("Textures/profile/Inv.png"); //��������� ���������
	texture[10].loadFromFile("Textures/profile/interface.png"); //��������� �������������
	//texture[11].loadFromFile("Textures/profile/Options.png"); //������ ��������
	texture[12].loadFromFile("Textures/profile/exit.png"); //������ ������
	texture[13].loadFromFile("Textures/profile/frameCharacter.png");
	texture[14].loadFromFile("Textures/profile/TestFon.jpg");
	texture[15] = texture[2];

	for (int j = 0; j < 20; j++)
		sprite[j].setTexture(texture[j]);

	sprite[1].setPosition(100, 100);
	sprite[2].setPosition(70, 300);
	sprite[3].setPosition(70, 350);
	sprite[4].setPosition(240, 200);
	sprite[5].setPosition(600, 300);
	sprite[6].setPosition(320, 270);
	sprite[7].setPosition(420, 270);
	sprite[8].setPosition(520, 270);
	sprite[9].setPosition(200, 100);
	sprite[10].setPosition(320, 70);
	sprite[11].setPosition(10, 500);
	sprite[12].setPosition(10, 10);
	sprite[15].setPosition(70, 420);
	
	//Text
	font.loadFromFile("ttf/CyrilicOld.ttf");

	for (int j = 0; j < 20; j++) {
		text[j].setFont(font);
		text[j].setPosition(0, 0);
		text[j].setCharacterSize(14);
		text[j].setFillColor(Color::White);
		text[j].setStyle(Text::Bold);
		text[j].setString("");
		values[j] = text[j];
	}
	text[0].setString("������: ");
	text[1].setString("������: ");
	text[2].setString("���� �������� �����: ");
	text[3].setString("���� 1 �����: ");
	text[4].setString("���� 2 �����: ");
	text[5].setString("���� 3 �����: ");
	text[6].setString("��������: ");
	text[7].setString("��������: ");
	text[8].setString("����������");
	text[9].setString("�������� ��������");
	text[10].setString("��� ���������");
	text[11].setString("��������");
	text[12].setString("���");
	text[13].setString("<");
	text[14].setString(">");
	text[15].setString("������� �����");
	//text[16].setString("ID: "); 
	//text[17].setString("PID: ");

	text[0].setPosition(600, 100);
	text[1].setPosition(600, 120);
	text[2].setPosition(340, 100); 
	text[3].setPosition(340, 125);
	text[4].setPosition(340, 150);
	text[5].setPosition(340, 175);
	text[6].setPosition(340, 200);
	text[7].setPosition(340, 225);
	text[8].setPosition(650, 50);
	text[9].setPosition(350, 20); text[9].setCharacterSize(20);
	text[10].setPosition(130, 80);
	text[11].setPosition(112, 303); text[11].setCharacterSize(20);
	text[12].setPosition(130, 353); text[12].setCharacterSize(20);
	text[13].setPosition(80, 170); text[13].setCharacterSize(28);
	text[14].setPosition(220, 170); text[14].setCharacterSize(28);
	text[15].setPosition(85, 425); text[15].setCharacterSize(20);
	//text[16].setPosition(600, 160); 
	//text[17].setPosition(600, 180);

	values[0].setPosition(text[2].getPosition().x + 150, text[2].getPosition().y);
	values[1].setPosition(text[3].getPosition().x + 100, text[3].getPosition().y);
	values[2].setPosition(text[4].getPosition().x + 100, text[4].getPosition().y);
	values[3].setPosition(text[5].getPosition().x + 100, text[5].getPosition().y);
	values[4].setPosition(text[6].getPosition().x + 75, text[6].getPosition().y);
	values[5].setPosition(text[7].getPosition().x + 75, text[7].getPosition().y);
	values[6].setPosition(text[0].getPosition().x + 70, text[0].getPosition().y);
	values[7].setPosition(text[1].getPosition().x + 60, text[1].getPosition().y);
	
	//my extraordinary design
	text[0].setColor(Color(108, 167, 207));
	text[1].setColor(Color(108, 167, 207));
	text[2].setColor(Color(108, 167, 207));
	text[3].setColor(Color(108, 167, 207));
	text[4].setColor(Color(108, 167, 207));
	text[5].setColor(Color(108, 167, 207));
	text[6].setColor(Color(108, 167, 207));
	text[7].setColor(Color(108, 167, 207));
	//----

	//������������� ����������
	for (int j = 0; j < 3; j++) {
		CurSAElements[j] = new SAElementInv();
		CurSAElements[j]->sprite.setTexture(texture[6]);
		CurSAElements[j]->sprite.setPosition(400, 100);
	}
	CurCh = new Character();
	CurCh->frame.setTexture(texture[13]);
	CurCh->id = 0; 

	CurMap = new infoMap(); 
	CurMap->name = "Valley"; //�.� ����� ����� �� ���������
} 

void Profile::menu(RenderWindow& window)
{
	updateSA();
	updateCharacter();
	
	while (true)
	{
		int menunum = 0;

		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			else if (events.type == Event::MouseWheelMoved);
		}
		
		//----������������ �����
		if (IntRect(70, 300, 170, 40).contains(Mouse::getPosition(window))) { menunum = 1; } //��������
		if (IntRect(70, 350, 170, 40).contains(Mouse::getPosition(window))) { menunum = 2; } //���� � ���
		if (IntRect(240, 200, 70, 70).contains(Mouse::getPosition(window))) { menunum = 3; } //��������� ������
		if (IntRect(600, 300, 70, 70).contains(Mouse::getPosition(window))) { menunum = 4; } //��������� ��

		if (IntRect(80, 170, 30, 30).contains(Mouse::getPosition(window))) { menunum = 8; } //��� ��������� ������������ ���������
		if (IntRect(220, 170, 30, 30).contains(Mouse::getPosition(window))) { menunum = 9; } //
		if (IntRect(70, 420, 170, 40).contains(Mouse::getPosition(window))) { menunum = 11; } //���� ������ �����

		if (IntRect(sprite[12].getPosition().x, sprite[12].getPosition().y, 45, 45).contains(Mouse::getPosition(window)))  
			menunum = 10;  //������ ������

		for (int j = 0; j < 3; j++) //������� 3 �����������
			if (IntRect(330 + j * 100, 280, 100, 100).contains(Mouse::getPosition(window))) 
				menunum = 5 + j; //��������� ��
		
		if (Mouse::isButtonPressed(Mouse::Left)) //��� ������� 
		{
			if (menunum == 1) redactor.menu(window); //����� � ��������
			if (menunum == 3) Character_Inventor(window);
			if (menunum == 4) SA_Inventor(window, 0);
			if (menunum == 11) SelectingMap(window);

			if (menunum == 5) SA_Inventor(window, 1); //3 ������� �����������
			if (menunum == 6) SA_Inventor(window, 2);
			if (menunum == 7) SA_Inventor(window, 3);

			if (menunum == 2) { //����� � ������ (����������������� �������� ������ ����� ��� ���������)
				engine.LoadMap(CurMap->name);
				engine.start(window);
			} 

			if (menunum == 8) {

				for (int j = 1; j < PElements.size(); j++)
					if (PElements[j]->id == CurCh->id && PElements[j - 1]->id != 0)
					{
						CurCh->id = PElements[j - 1]->id; break;
					}
				
				saveOptions_Pr(CurCh->id);
				updateCharacter();
				Sleep(200);
			}

			if (menunum == 9) {

				for (int j = 0; j < PElements.size(); j++)
					if (j + 1 < PElements.size()) 
						if (PElements[j]->id == CurCh->id && PElements[j + 1]->id != 0)
						{ 
							CurCh->id = PElements[j + 1]->id; break; 
						}
				
				saveOptions_Pr(CurCh->id); 
				updateCharacter();
				Sleep(200);
			}

			if (menunum == 10) exit(0); //�����
		}
		
		//Draw - display
		window.clear();

		window.draw(sprite[0]); //���

		window.draw(sprite[10]); //��������� ��� �������������
		window.draw(sprite[11]); //������ ��������
		window.draw(sprite[12]); //������ ������

		window.draw(sprite[2]); //������
		window.draw(sprite[3]); 
		window.draw(sprite[15]);

		for (int j = 0; j < 20; j++) {
			window.draw(text[j]); //����� ����� ������
			window.draw(values[j]);
		}

		window.draw(sprite[6]); //3 ������ ����������� (�����)
		window.draw(sprite[7]);
		window.draw(sprite[8]);

		window.draw(sprite[4]); //��������� ����������
		window.draw(sprite[5]); //��������� ������ ������������

		window.draw(sprite[1]); //��� ���������
		window.draw(CurCh->sprite);

		window.display();
	}
}

void Profile::SA_Inventor(RenderWindow& window, int arg)
{
	updateSA(); //�������� ������ ������ ������������

	sprite[9].setScale(0.6, 1.02);
	sprite[9].setPosition(395, 100);

	Sleep(200);

	while (true)
	{
		int menunum = 0;
		
		Event events;
		while (window.pollEvent(events))
			if (events.type == Event::MouseWheelMoved) //scrolling
			{
				for (int j = 0; j < SAElements.size(); j++) {
					SAElements[j]->sprite.move(0, events.mouseWheel.delta * 10);
					SAElements[j]->idSA.move(0, events.mouseWheel.delta * 10);
					SAElements[j]->name.move(0, events.mouseWheel.delta * 10);
				}
			}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
			break;  //�����
		if (IntRect(sprite[9].getPosition().x, sprite[9].getPosition().y, 510, 410).contains(Mouse::getPosition(window)))
			menunum = -1;  //����� ��� ������� �� ������� ��� �����

		for (int j = 0; j < SAElements.size(); j++) 
			if (IntRect(SAElements[j]->sprite.getPosition().x, SAElements[j]->sprite.getPosition().y, 300, 70).contains(Mouse::getPosition(window)))
				menunum = SAElements[j]->id; //menunum ����� ����� ���������� idSA
		
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menunum == 0) { break; }   //����� ��� ������� �� ������� ��� �����

			if (menunum != -1) //������� �����������
				if (CurSAElements[arg - 1]->id != menunum) { //��������� �� ���������� �� ��� ��� �����������
					CurSAElements[arg - 1]->id = menunum;
					saveOptions_SA(arg - 1, menunum); //��������� ���������
					Sleep(200); break;
				}
			
		}
		
		//-----------------------------------------------------
		window.draw(sprite[9]); //���

		for (int j = 0; j < SAElements.size(); j++)
			if (SAElements[j]->sprite.getPosition().y >= 170 && 
				SAElements[j]->sprite.getPosition().y <= 450) //������� ���������
			{
				window.draw(SAElements[j]->sprite);
				window.draw(SAElements[j]->name);
				window.draw(SAElements[j]->idSA);
			}

		if (arg != 0) { //������� ����������� �� ��������� ����������
			window.draw(CurSAElements[arg - 1]->sprite);
			window.draw(CurSAElements[arg - 1]->idSA);
			window.draw(CurSAElements[arg - 1]->name);
		}

		window.display();
	}
}

void Profile::Character_Inventor(RenderWindow& window)
{
	updateCharacter();

	sprite[9].setScale(1, 1);
	sprite[9].setPosition(250, 100);

	while (true)
	{
		int menunum = 0;

		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::MouseWheelMoved)
			{
				if (events.mouseWheel.delta == 1) {} //��� ���� ����� �������� ��������
				if (events.mouseWheel.delta == -1) {} //��������� ������ �� �����������

				for (int j = 0; j < PElements.size(); j++)
				{
					PElements[j]->frame.move(0, events.mouseWheel.delta * 10);
					PElements[j]->sprite.move(0, events.mouseWheel.delta * 10);
				}
			}
		}
		
		if (IntRect(sprite[9].getPosition().x, sprite[9].getPosition().y, 530, 410).contains(Mouse::getPosition(window))) 
			menunum = 1;  //�����

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menunum == 0) { break; } //�����
		}
		
		window.draw(sprite[9]); //���
		
		for (int j = 0; j < PElements.size(); j++)
			if (PElements[j]->sprite.getPosition().y >= 100 &&
				PElements[j]->sprite.getPosition().y <= 345) { //���������� ������� ������ � ���� ��������� ����
				window.draw(PElements[j]->frame);
				window.draw(PElements[j]->sprite);
			}

		window.display();
	}
}

void Profile::SelectingMap(RenderWindow& window)
{
	updateMaps();

	for (int j = 0; j < Maps.size(); j++) {
		Maps[j]->sprite.setPosition(250, j * 80 + 100);
		Maps[j]->text.setPosition(300, j * 80 + 120);
		Maps[j]->text.setColor(Color::White);
		Maps[j]->text.setCharacterSize(18);
	}

	while (true)
	{
		int mapnum = -1;

		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			if (events.type == Event::MouseWheelMoved) //scrolling
			{
				for (int j = 0; j < Maps.size(); j++) {
					Maps[j]->sprite.move(0, events.mouseWheel.delta * 10);
					Maps[j]->text.move(0, events.mouseWheel.delta * 10);
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) break; //exit

		for (int j = 0; j < Maps.size(); j++)
			if (IntRect(Maps[j]->sprite.getPosition().x, Maps[j]->sprite.getPosition().y, 350, 75).contains(Mouse::getPosition(window)))
				mapnum = j;
		
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (mapnum != -1) {
				CurMap = Maps[mapnum];

				for (int j = 0; j < Maps.size(); j++)
					Maps[j]->sprite.setColor(Color::White);
				Maps[mapnum]->sprite.setColor(Color(0, 0, 0, 120));
				Sleep(200);
			}
		}

		window.clear();
		window.draw(sprite[14]);

		for (int j = 0; j < Maps.size(); j++) {
			window.draw(Maps[j]->sprite);
			window.draw(Maps[j]->text);
		}

		window.display();
	}
}

void Profile::saveOptions_SA(int numb, int id)
{
	TiXmlDocument object("Save/account.xml");
	if (!object.LoadFile()) return;

	TiXmlElement* sizeQ = object.FirstChildElement("account")->FirstChildElement("character");
	
	while (true)
	{
		if (sizeQ->Attribute("id") == to_string(CurCh->id)) break; //���������� �� ���������� � ������� ������� id (�� saveCharacter)

		if (!(sizeQ = sizeQ->NextSiblingElement("character"))) return; //���� ���������� ��� ������ �������
	}
	sizeQ = sizeQ->FirstChildElement("saveSA");

	if (numb == 0) sizeQ->SetAttribute("Numb1", id);
	if (numb == 1) sizeQ->SetAttribute("Numb2", id);
	if (numb == 2) sizeQ->SetAttribute("Numb3", id);

	object.SaveFile();
}

void Profile::saveOptions_Pr(int idCh_) 
{
	TiXmlDocument object("Save/account.xml");
	if (!object.LoadFile()) return;

	TiXmlElement* sizeQ = object.FirstChildElement("account")->FirstChildElement("saveCharacter");
	sizeQ->SetAttribute("id", idCh_);

	object.SaveFile();
}

void Profile::updateSA()
{
	TiXmlDocument object("Save/account.xml");
	if (!object.LoadFile()) return;

	//�������� SA � ����� ����������
	TiXmlElement* SATiny;
	SATiny = object.FirstChildElement("account")->FirstChildElement("inventor");

	TiXmlElement* SAElement; //������ ��� SA ��������
	SAElement = SATiny->FirstChildElement("SA");

	for (int j = 0; j < SAElements.size(); j++) //�����
		delete SAElements[j];
	SAElements.clear(); 

	while(true) { //load all SA
		SAElements.push_back(new SAElementInv());

		SAElements.back()->idSA = text[0]; //setFont setSize ...
		SAElements.back()->name = text[0];

		SAElements.back()->sprite.setTexture(texture[6]);
		SAElements.back()->idSA.setString(SAElement->Attribute("id"));
		SAElements.back()->name.setString(SAElement->Attribute("name"));
		SAElements.back()->id = atoi(SAElement->Attribute("id"));
		
		if (!(SAElement = SAElement->NextSiblingElement("SA")))
			break; //���� ��������� ������ ��� �������
	}

	for (int j = 0; j < SAElements.size(); j++) {
		SAElements[j]->sprite.setPosition(400, 100 + j * 70);
		SAElements[j]->idSA.setPosition(500, 130 + j * 70);
		SAElements[j]->name.setPosition(500, 110 + j * 70);

		SAElements[j]->idSA.move(0, 70);
		SAElements[j]->name.move(0, 70);
		SAElements[j]->sprite.move(0, 70);
	}

	//���������� ������� ��������� SA
	TiXmlElement* uk = object.FirstChildElement("account")->FirstChildElement("inventor")->FirstChildElement("SA");
	TiXmlElement* infoCharacter;

	infoCharacter = object.FirstChildElement("account")->FirstChildElement("saveCharacter");
	string idCh = infoCharacter->Attribute("id");

	infoCharacter = object.FirstChildElement("account")->FirstChildElement("character");

	for (int j = 0; j < 50; j++) //����� ������ id
	{
		if (idCh == infoCharacter->Attribute("id")) break; //���� ������ ��� ���� �� ��������� ����

		if (!(infoCharacter = infoCharacter->NextSiblingElement("character")))
			break; //���� ��������� ������ ��� �������
	}

	infoCharacter = infoCharacter->FirstChildElement("saveSA"); 

	string id[3];
	id[0] = infoCharacter->Attribute("Numb1");
	id[1] = infoCharacter->Attribute("Numb2");
	id[2] = infoCharacter->Attribute("Numb3");

	for (int j = 0; j < 3; j++) { //load current free selected SA

		uk = object.FirstChildElement("account")->FirstChildElement("inventor")->FirstChildElement("SA");

		while (true) {
			if (id[j] == uk->Attribute("id"))
				break;

			if (!(uk = uk->NextSiblingElement("SA")))
				return;
		}

		CurSAElements[j]->name = text[0]; //inisialization
		CurSAElements[j]->idSA = text[0];

		CurSAElements[j]->name.setString(uk->Attribute("name"));
		CurSAElements[j]->idSA.setString(id[j]);
		CurSAElements[j]->id = atoi(id[j].c_str());

		CurSAElements[j]->name.setPosition(500, 110);
		CurSAElements[j]->idSA.setPosition(500, 130);
	}
}

void Profile::updateCharacter()
{
	TiXmlDocument object("Save/account.xml");
	if (!object.LoadFile()) return;

	TiXmlElement* infoCharacter = object.FirstChildElement("account")->FirstChildElement("character");

	for (int j = 0; j < PElements.size(); j++) //�����
		delete PElements[j];
	PElements.clear();

	for (int j = 0; j < 50; j++) //load all persons
	{
		PElements.push_back(new Character);
		PElements[j]->frame.setTexture(texture[13]);
		PElements[j]->frame.setPosition(300 + j * 112, 100 + 0 * 161);
		PElements[j]->sprite = TT.ProfileCharacter[atoi(infoCharacter->Attribute("id"))];
		PElements[j]->sprite.setPosition(300 + j * 112, 100 + 0 * 161);
		PElements[j]->width = atoi(infoCharacter->Attribute("width"));
		PElements[j]->height = atoi(infoCharacter->Attribute("height"));
		PElements[j]->id = atoi(infoCharacter->Attribute("id"));

		TiXmlElement* currentCh = infoCharacter;
		PElements[j]->normAt = atoi(currentCh->FirstChildElement("AttackDamage")->Attribute("normAt"));
		PElements[j]->At1 = atoi(currentCh->FirstChildElement("AttackDamage")->Attribute("At1"));
		PElements[j]->At2 = atoi(currentCh->FirstChildElement("AttackDamage")->Attribute("At2"));
		PElements[j]->At3 = atoi(currentCh->FirstChildElement("AttackDamage")->Attribute("At3"));
		PElements[j]->speed = atoi(currentCh->FirstChildElement("Speed")->Attribute("Value"));
		PElements[j]->xp = atoi(currentCh->FirstChildElement("Xp")->Attribute("Value"));
		
		if (!(infoCharacter = infoCharacter->NextSiblingElement("character")))
			break; //���� ��������� ������ ��� �������
	}

	infoCharacter = object.FirstChildElement("account")->FirstChildElement("saveCharacter");
	string idCh = infoCharacter->Attribute("id"); //id current character

	for (int j = 0; j < PElements.size(); j++)
		if (atoi(idCh.c_str()) == PElements[j]->id) {
			CurCh = PElements[j]; 
			CurCh->sprite = TT.ProfileCharacter[CurCh->id]; //setPosition
			break;
		}

	values[6].setString(to_string(CurCh->width));
	values[7].setString(to_string(CurCh->height));
	text[10].setString(CurCh->name); //update name character
	values[0].setString(to_string(CurCh->normAt));
	values[1].setString(to_string(CurCh->At1));
	values[2].setString(to_string(CurCh->At2));
	values[3].setString(to_string(CurCh->At3));
	values[4].setString(to_string(CurCh->xp));
	values[5].setString(to_string(CurCh->speed));
	text[9].setString(CurCh->name); //update name account
}

void Profile::updateMaps()
{
	//load maps
	WIN32_FIND_DATAA findData;
	HANDLE hf;

	for (int j = 0; j < Maps.size(); j++) //����� � ��������
		delete Maps[j];
	Maps.clear();

	hf = FindFirstFileA("maps\\*", &findData);

	if (hf == INVALID_HANDLE_VALUE)
		cout << "Cannot find file" << endl;
	else {
		FindNextFileA(hf, &findData);
		FindNextFileA(hf, &findData);

		do {
			Maps.push_back(new infoMap);
			Maps.back()->name = findData.cFileName;
			Maps.back()->text = text[0];
			Maps.back()->text.setString(Maps.back()->name);
			Maps.back()->sprite = sprite[2];
			Maps.back()->sprite.setScale(2,2);

			//cout << findData.cFileName << endl;
		} while (FindNextFileA(hf, &findData));
	}
	FindClose(hf);
}
