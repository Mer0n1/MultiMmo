#include "SubEntity.h"
#include "GameWorld.h"


Enemy::Enemy(string name1, int id_, int IDO_) 
{
	name = name1;
	speedConst = 5;
	speed = 1; 
	hp = 100;
	max_hp = hp;
	x = 300;
	y = 300;
	visibility = 400;
	DistanceFromOpponent = 8; 
	pidOp = 0;
	jOp = 0;
	id = id_;
	srand(::time(0));
	pid = rand() % 10000 + 1; 
	IDO = IDO_;
	rations.at1 = 0;
	rations.at2 = 0;
	rations.at3 = 0;
	rations.normAt = 0;
	rations.currentAt = 0;
	life = true;
	memset(x_move, 0, 20 * 4);
	memset(y_move, 0, 20 * 4);
	hb.saveSetScale(1, 1); //настраиваем полоску жизней под бота

	group = new Group_Rpg; 
	group->vec.push_back(this); //добавим группу и себя в нее
	
	DownloadInformation(); //скачивание информации из библиотеки
	world->addEntity(this); //автоматически при создании добавляем его в список мира
	
	TFPack->ModelCharacter[id].setTextureRect(IntRect(Width * 2, 0, Width, Height)); //IntRect - соответствует типу сборки модели
	gamer = TFPack->ModelCharacter[id]; //загружаем модель
	
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
	pr.group = group;

	AttackMode.setPr(&pr);
	AttackMode.inisialization(); //инициализируем всю систему
}

Enemy::~Enemy()
{
}

void Enemy::update()
{
	if (name == "EasyEnemy") {//для персонажа с таким именем логика будет такой
		
		gamer.setPosition(x, y);
		hb.setPosition(x - 10, y - 10);
		hb.setProgress((float)hp / (float)max_hp);
		hb.setHp(hp, max_hp); 

		controlEnemy();
		interactionWithMap(); //связь с картой
		go_move(); //движение бота
		ai(); //поведение к игроку
		
		if (hp <= 0) { life = false; gamer.setColor(Color::Black); }
	}
}

void Enemy::go_move()
{
	if (!moving) return;

	if (x <= (int)x_move[0] + 5 & x >= (int)x_move[0] - 5 &&
		y <= (int)y_move[0] + 5 & y >= (int)y_move[0] - 5) { 
		left = false; 
		right = false; 
		forward = false;
		back = false;
		moving = false; 
		return;
	}
	left = 0, 
	right = 0, 
	forward = 0, 
	back = 0; //сброс
	
	int nx = 0, ny = 0; //направления
	if (x_move[0] > x) nx = 1; else nx = -1; //по графику
	if (y_move[0] > y) ny = 1; else ny = -1;

	//если по системе булевых num то используем тайлы 32 на 32
	if (nx == 1) right = true; else left = true;
	if (ny == 1) back = true; else forward = true;

	//признак отсутствия движения
	if (x_move[0] + 1 == x) nx = 0;
	if (y_move[0] + 1 == y) ny = 0;
	if (nx == 0) { left = false; right = false; }
	if (ny == 0) { forward = false; back = false; }
}

void Enemy::ai()
{
	bool sight = false; //видимость противника
	int i1 = 0, j1 = 0; //дистанция до цели
	int opX = 0, opY = 0; //координаты противника
	
	//-----------------------Перемещение
	for (int j = 0; j < world->getSize(); j++)
		if (world->getEntity(j)->getPid() != pid && CheckGroup(j) &&
			world->getEntity(j)->getPos().x > x - visibility && world->getEntity(j)->getPos().x < x + visibility &&
			world->getEntity(j)->getPos().y > y - visibility && world->getEntity(j)->getPos().y < y + visibility)
		{ 
			sight = true;
			i1 = world->getEntity(j)->getPos().y;
			j1 = world->getEntity(j)->getPos().x;
			pidOp = world->getEntity(j)->getPid(); //сохраняем подайди выбранной ботом цели
			jOp = j;
			break;
		}

	if (tether) //обработка привязки к местности
		if (x < Point[0] - DistantionPoint || x > Point[0] + DistantionPoint ||
			y < Point[1] - DistantionPoint || y > Point[1] + DistantionPoint) //не дальше DistantionPoint от точки
		{
			x_move[0] = Point[0]; 
			y_move[0] = Point[1]; 
			//hp = max_hp; //как вариант - восстановление хп если выходит за границу
			moving = true;
			sight = false;
		}
	
	if (!sight) return; 
	if (BlockAi) return; //выходим если движение типа saveDistantion заблочено

	opX = j1; 
	opY = i1; //сохраняем изначальные координаты противника

	i1 = i1 - y;  //сохраняем отношение координат противника и себя
	j1 = j1 - x + (Width / 2); //i - это y, j1 - х

	//сколько нужно отнять/прибравить чтобы получилось DistanceFromOpponent? и какую координату?
	int xx = 0, yy = 0; 
	if (opX > x) xx = j1 - DistanceFromOpponent * 32; else xx = j1 + DistanceFromOpponent * 32; //чтобы он вообще не приближался 
	if (opY > y) yy = i1 - DistanceFromOpponent * 32; else yy = i1 + DistanceFromOpponent * 32; //к игроку

	//прибавляем
	x_move[0] = x + xx;
	y_move[0] = y + yy;
	moving = true;  
}

void Enemy::controlEnemy()
{
	if (*time < controltime) return;
	controltime = *time + 0.01;
	
	//движение
	if (forward) {
		y -= speed;
		gamer.setTextureRect(IntRect(Width * 6, 0, Width, Height));
	}
	if (right) {
		x += speed;
		gamer.setTextureRect(IntRect(0, 0, Width, Height));
	}
	if (left) {
		x -= speed;
		gamer.setTextureRect(IntRect(Width * 4, 0, Width, Height));
	}
	if (back) {
		y += speed;
		gamer.setTextureRect(IntRect(Width * 2, 0, Width, Height));
	}
	
	//боковое движение
	if (forward && left) //вверх-лево
		gamer.setTextureRect(IntRect(Width * 5, 0, Width, Height));
	if (forward && right) //вверх-право
		gamer.setTextureRect(IntRect(0, Height, Width, Height));
	if (back && left) //вниз-лево
		gamer.setTextureRect(IntRect(Width * 3, 0, Width, Height));
	if (back && right) //вниз-право
		gamer.setTextureRect(IntRect(Width * 1, 0, Width, Height));

}

void Enemy::interactionWithMap()
{
	if (!Map->interactionMap(x, y, Height, Width)) return; //если препятствия нет выходим
	
	if (back)
		y -= speedConst;
	if (forward)
		y += speedConst;
	if (right)
		x -= speedConst;
	if (left)
		x += speedConst;
}

void Enemy::updateHp(int atck)
{
	hp -= atck; 
}

bool Enemy::CheckGroup(int n)
{
	if (group == NULL) return true; //если группы не сущ выходим
	
	for (int j = 0; j < group->vec.size(); j++)
		if (group->vec[j]->getPid() != pid &&
			world->getEntity(n)->getPid() == group->vec[j]->getPid())
			return false;

	return true; 
}

void Enemy::DownloadInformation()
{
	TiXmlDocument object("Save/LibraryNPC.xml");
	object.LoadFile();

	TiXmlElement* Q;
	Q = object.FirstChildElement("Library");

	TiXmlElement* Person;
	Person = Q->FirstChildElement("character");

	while (true)
	{
		if (Person->Attribute("id") == to_string(id) && Person->Attribute("IDO") == to_string(IDO))
			break;

		if (!(Person = Person->NextSiblingElement("character")))
			return;
	}
	
	Width = atoi(Person->Attribute("width")); //загруза характеристик
	Height = atoi(Person->Attribute("height"));
	hp = max_hp = atoi(Person->Attribute("hp"));
	speed = atoi(Person->Attribute("speed"));
	visibility = atoi(Person->Attribute("visibility"));
	DistanceFromOpponent = atoi(Person->Attribute("DistanceFromOpponent"));
	
	//Загружаем информацию о уроне
	if (Person->FirstChildElement("AttackDamage")) { //ничего страшного если персонаж не имеет этого параметра
		rations.at1 = atoi(Person->FirstChildElement("AttackDamage")->Attribute("At1"));
		rations.at2 = atoi(Person->FirstChildElement("AttackDamage")->Attribute("At2"));
		rations.at3 = atoi(Person->FirstChildElement("AttackDamage")->Attribute("At3"));
		rations.normAt = atoi(Person->FirstChildElement("AttackDamage")->Attribute("normAt"));
		rations.currentAt = rations.normAt;
	}

	//Загружаем SA
	TiXmlElement* SAElement = Person->FirstChildElement("SA"); //загрузка характеристик атак

	for (int numb = 0; numb < 3; numb++) //3 SA
	{
		if (numb != 0)
			if (!(SAElement = SAElement->NextSiblingElement("SA"))) //если SA нет больше выходим
				return;

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
		//------------------------------------Переводим string в массив
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
		//-----------------------------------------------------------
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
			SAAttack.SizeTile = atoi(Description->Attribute("Size"));
		
		//вводим VidE (если есть)
		if (SAElement->FirstChildElement("mobs")) {
			TiXmlElement* VidE = SAElement->FirstChildElement("mobs");
			SAAttack.VidE = atoi(VidE->Attribute("Vide"));
			SAAttack.tVide = atoi(VidE->Attribute("tVide"));
			int qVide = atoi(VidE->Attribute("qVide")); 

			if (qVide != 0 && SAAttack.VidE != 0) //если атака многообьектная
				for (int j = 0; j < atoi(VidE->Attribute("qVide")); j++) 
					AttackMode.addObj(AttackMode.getPr(), AttackMode.getSAAttack(numb));
		}
		AttackMode.setSAAttack(&SAAttack, numb);
	}
}

void Enemy::setGroup(Group_Rpg* group_)
{
	group = group_;
	group_->vecAS.push_back(&AttackMode); //добавляем недостающий AttackMode

	AttackMode.getPr()->group = group;
	AttackMode.inisialization();
}

void Enemy::setTether(int DistantionPoint_)
{
	tether = true;
	Point[0] = x; //точка привязки будет равна начальным координатам
	Point[1] = y;
	DistantionPoint = DistantionPoint_; 
}

void Enemy::setVisibility(int value)
{
	visibility = value;
}

void Enemy::setDistantionForOpponent(int value)
{
	DistanceFromOpponent = value; 
}


