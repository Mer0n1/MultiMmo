#include "SubEntity.h"
#include "GameWorld.h"


Player::Player()
{
	hp = 0, max_hp = 0;
	speed = 0;
	x = 0; y = 0;
	srand(::time(0));
	pid = rand() % 10000 + 1;
	time_save = 0;
	speedConst = 0;

	readyPerson(); //загрузка характеристик с соответствием с его айди
	world->addEntity(this); //автоматически при создании добавляем его в список мира
	
	TFPack->ModelCharacter[id].setTextureRect(IntRect(0, 0, Width, Height));
	gamer = TFPack->ModelCharacter[id]; //загрузка модели

	rech = new RechargeAttack[3]; 
	for (int j = 0; j < 3; j++)
		rech[j] = { j + 1, AttackMode.getSAAttack(j)->recharge };

	playerPr pr;
	pr.x = &x;
	pr.y = &y;
	pr.posX = &pixelPos->x;
	pr.posY = &pixelPos->y;
	pr.hp = &hp;
	pr.maxHp = &max_hp;
	pr.Height = Height;
	pr.Width = Width;
	pr.id = id;
	pr.pid = pid;
	pr.at1 = &rations.at1;
	pr.at2 = &rations.at2;
	pr.at3 = &rations.at3;
	pr.normAt = &rations.normAt;
	pr.currentAt = &rations.at1;

	AttackMode.setPr(&pr);
	AttackMode.inisialization(); //инициализируем всю систему
}

Player::~Player()
{
	delete group;
	//delete rech; //rech должен быть очищен еще в интерфейсе
}

void Player::update()
{	
	gamer.setPosition(x, y);
	hb.setProgress((float)hp / (float)max_hp);
	hb.setHp(hp, max_hp);

	controlPlayer(); //управление персонажем
	interactionWithMap();
	AttackMode.go_attack();

	if (hp <= 0) { life = false; exit(0); }
}

void Player::interactionWithMap()
{
	if (!Map->interactionMap(x, y, Height, Width)) return; //если препятствия нет выходим

	if (dy > 0)
		y -= speedConst;
	if (dy < 0)
		y += speedConst;
	if (dx > 0)
		x -= speedConst;
	if (dx < 0)
		x += speedConst;
}

void Player::controlPlayer()
{

	switch (dir) //направление движения
	{
	case 0: dx = speed; dy = 0; break;
	case 1: dx = -speed; dy = 0; break;
	case 2: dx = 0; dy = speed; break;
	case 3: dx = 0; dy = -speed; break;
	case 4: dx = speed; dy = speed; break; //боковые вверх-вправо
	case 5: dx = -speed; dy = -speed; break; //вверх-влево
	case 6: dx = -speed; dy = speed; break; //вниз - влево
	case 7: dx = speed; dy = -speed; break; //вниз -вправо
	}

	x += dx;
	y += dy;
	speed = 0;
	//-------------------------------------
	if (Keyboard::isKeyPressed(Keyboard::Num1)) { AttackMode.setNum(1, true); rech[0].StartTimer(); }//включаем атаку
	if (Keyboard::isKeyPressed(Keyboard::Num2)) { AttackMode.setNum(2, true); rech[1].StartTimer(); }
	if (Keyboard::isKeyPressed(Keyboard::Num3)) { AttackMode.setNum(3, true); rech[2].StartTimer();  }
	if (Mouse::isButtonPressed(Mouse::Right))   { AttackMode.setNum(4, true); }
	//-------------------------------------

	if (!life) { return; } //если игрок мертв блокируем управление
	if (*time < sleeptime) { return; }
	
	if (Keyboard::isKeyPressed(Keyboard::W)) { //вверх
		gamer.setTextureRect(IntRect(Width * 6, 0, Width, Height));
		speed = 5; dir = 3;
		sleeptime = *time + 0.01; //Sleep(10);
	}

	if (Keyboard::isKeyPressed(Keyboard::D)) { //на право
		gamer.setTextureRect(IntRect(0, 0, Width, Height)); 
		speed = 5; dir = 0;
		sleeptime = *time + 0.01; //Sleep(10);
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) { //налево
		gamer.setTextureRect(IntRect(Width * 4, 0, Width, Height)); 
		speed = 5; dir = 1;
		sleeptime = *time + 0.01; //Sleep(10);
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) { //вниз
		gamer.setTextureRect(IntRect(Width * 2, 0, Width, Height)); 
		speed = 5; dir = 2;
		sleeptime = *time + 0.01; //Sleep(10);
	}

	if (Keyboard::isKeyPressed(Keyboard::S) && //боковая хотьба
		Keyboard::isKeyPressed(Keyboard::D)) {
		gamer.setTextureRect(IntRect(Width * 1, 0, Width, Height));
		speed = 3; dir = 4;
	}
	if (Keyboard::isKeyPressed(Keyboard::W) &&
		Keyboard::isKeyPressed(Keyboard::A)) {
		gamer.setTextureRect(IntRect(Width * 5, 0, Width, Height)); 
		speed = 3; dir = 5;
	}
	if (Keyboard::isKeyPressed(Keyboard::A) &&
		Keyboard::isKeyPressed(Keyboard::S)) {
		gamer.setTextureRect(IntRect(Width * 3, 0, Width, Height)); 
		speed = 3; dir = 6;
	}
	if (Keyboard::isKeyPressed(Keyboard::W) &&
		Keyboard::isKeyPressed(Keyboard::D)) {
		gamer.setTextureRect(IntRect(0, Height, Width, Height)); 
		speed = 3; dir = 7;
	}

	view->setCenter(x + (Width / 2), y + (Height / 2)); //балансируем экран
	Map->limitMap(x, y, *view);
}

void Player::updateHp(int atck)
{
	hp -= atck; 
}

RechargeAttack* Player::getModuleRA() //возвращаем модуль интерфейса 
{
	return rech;
}

void Player::readyPerson()
{

	//вытаскиваем информацию каждой атаки
	TiXmlDocument object("Save/account.xml");
	object.LoadFile();

	//загрузка данных о персонаже
	TiXmlElement* infoCharacter;
	infoCharacter = object.FirstChildElement("account")->FirstChildElement("saveCharacter");

	string idCh = infoCharacter->Attribute("id");
	infoCharacter = object.FirstChildElement("account")->FirstChildElement("character");

	for (int j = 0; j < 50; j++)
	{
		if (idCh == infoCharacter->Attribute("id")) break; //если найдем наш айди то сохраняем путь

		if (!(infoCharacter = infoCharacter->NextSiblingElement("character")))
			break; //если элементов больше нет выходим
	}
	//загрузка характеристик 
	name = infoCharacter->Attribute("name");
	Width = atoi(infoCharacter->Attribute("width")); //изменяем характеристики игрока
	Height = atoi(infoCharacter->Attribute("height"));
	id = atoi(infoCharacter->Attribute("id"));
	hp = atoi(infoCharacter->FirstChildElement("Hp")->Attribute("Value"));
	max_hp = atoi(infoCharacter->FirstChildElement("Hp")->Attribute("MaxHp"));
	speed = atoi(infoCharacter->FirstChildElement("Speed")->Attribute("Value"));
	rations.at1 = atoi(infoCharacter->FirstChildElement("AttackDamage")->Attribute("At1"));
	rations.at2 = atoi(infoCharacter->FirstChildElement("AttackDamage")->Attribute("At2"));
	rations.at3 = atoi(infoCharacter->FirstChildElement("AttackDamage")->Attribute("At3"));
	rations.normAt = atoi(infoCharacter->FirstChildElement("AttackDamage")->Attribute("normAt"));
	speedConst = speed;

	//----------------------загрузка SA
	//загружаем сохранения выбранных атак (которые уже стоят там)
	string Numb1, Numb2, Numb3, nool;
	TiXmlElement* character = infoCharacter->FirstChildElement("saveSA");
	Numb1 = character->Attribute("Numb1");
	Numb2 = character->Attribute("Numb2");
	Numb3 = character->Attribute("Numb3");
	
	if (Numb1 != nool) downloadSA(0, atoi(Numb1.c_str())); //загружаем информацию в ячейку
	if (Numb2 != nool) downloadSA(1, atoi(Numb2.c_str()));
	if (Numb3 != nool) downloadSA(2, atoi(Numb3.c_str()));
}


void Player::downloadSA(int numb, int id) //загрузка данных SA персонажа
{
	TiXmlDocument object("Save/account.xml");
	object.LoadFile();

	TiXmlElement* sizeQ = object.FirstChildElement("account")->FirstChildElement("inventor");

	TiXmlElement* SAElement; //читаем абилки
	SAElement = sizeQ->FirstChildElement("SA");

	for (int j = 0; j < 50; j++) {
		if (to_string(id) == SAElement->Attribute("id")) break; 

		if (!(SAElement = SAElement->NextSiblingElement("SA")))
			break;
	}
	
	//переводим сперва в string и после во float
	TiXmlElement* DelayTix = SAElement->FirstChildElement("delay");
	TiXmlElement* durationTix = SAElement->FirstChildElement("duration");
	TiXmlElement* TileTix = SAElement->FirstChildElement("Tile");
	TiXmlElement* Description = SAElement->FirstChildElement("Description");
	
	string delayStr = DelayTix->Attribute("Value");
	string durationStr = durationTix->Attribute("Value");
	string TileX = TileTix->Attribute("ValueX");
	string TileY = TileTix->Attribute("ValueY");
	
	CurrentAttack SAAttack;
	//---------------------------------------------------
	//Переведем string в массив и сохраним в систему атаки
	interpretation inter;
	int Size = atoi(Description->Attribute("QuanityTile"));
	int iSize = Size * sizeof(int);
	int fSize = Size * sizeof(float);
	
	inter = delayStr; 
	inter.toFloatMassv(&SAAttack.delay[0], fSize);

	inter = durationStr;
	inter.toFloatMassv(&SAAttack.duration[0], fSize);

	int uk[100]; //если есть двумерный массив разделяем его на два

	inter = TileX;
	inter.toIntMassv(&uk[0], iSize);

	for (int j = 0; j < Size; j++)
		SAAttack.Tile[j][0] = uk[j];

	inter = TileY;
	inter.toIntMassv(&uk[0], iSize);

	for (int j = 0; j < Size; j++)
		SAAttack.Tile[j][1] = uk[j];
	//------------------------------------------------------------------
	//Сохраняем характеристики описания
	
	string TypeAttack = Description->Attribute("TypeAttack");
	
	if (TypeAttack == "Do")
		SAAttack.type_attack = Do;
	if (TypeAttack == "dpsa")
		SAAttack.type_attack = dpsa;
	if (TypeAttack == "ida")
		SAAttack.type_attack = ida;
	
	SAAttack.id = id;
	SAAttack.numberSA = numb;
	SAAttack.distantion = atoi(Description->Attribute("Distantion"));
	SAAttack.FuncAttack = Description->Attribute("FuncAttack");
	SAAttack.recharge = atoi(Description->Attribute("Recharge"));
	SAAttack.QuanityTile = atoi(Description->Attribute("QuanityTile"));
	if (Description->Attribute("Size"))
		SAAttack.SizeTile = atoi(Description->Attribute("Size")); //тест

	//вводим VidE (если есть)
	if (SAElement->FirstChildElement("mobs")) {
		TiXmlElement* VidE = SAElement->FirstChildElement("mobs");

		SAAttack.VidE = atoi(VidE->Attribute("Vide"));
		SAAttack.tVide = atoi(VidE->Attribute("tVide"));
	}
	AttackMode.setSAAttack(&SAAttack, numb);
}


void Player::inisialization()
{
	readyPerson();
	AttackMode.inisialization();
}