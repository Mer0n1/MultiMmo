#include "SubEntity.h"
#include "GameWorld.h"


Enemy::Enemy(string name1, int id_, int IDO_) 
{
	name = name1;
	speedConst = 5;
	speed = 1; 
	xp = 100;
	max_Xp = xp;
	x = 300;
	y = 300;
	visibility = 400;
	DistanceFromOpponent = 8; 
	pidOp = 0;
	jOp = 0;
	id = id_;
	pid = rand() % 5000 + 1; 
	IDO = IDO_;
	rations.at1 = 0;
	rations.at2 = 0;
	rations.at3 = 0;
	rations.normAt = 0;
	rations.currentAt = 0;
	life = true;
	memset(x_move, 0, 20 * 4);
	memset(y_move, 0, 20 * 4);
	hb.saveSetScale(1, 1); //����������� ������� ������ ��� ����

	group = new Group_Rpg; 
	group->vec.push_back(this); //������� ������ � ���� � ���
	
	DownloadInformation(); //���������� ���������� �� ����������
	world->addEntity(this); //������������� ��� �������� ��������� ��� � ������ ����
	
	TFPack->ModelCharacter[id].setTextureRect(IntRect(Width * 2, 0, Width, Height)); //IntRect - ������������� ���� ������ ������
	gamer = TFPack->ModelCharacter[id]; //��������� ������
	
	playerPr pr;
	pr.x = &x;
	pr.y = &y;
	pr.posX = &pixelPos->x;
	pr.posY = &pixelPos->y;
	pr.xp = &xp;
	pr.maxXp = &max_Xp;
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
	AttackMode.inisialization(); //�������������� ��� �������
}

Enemy::~Enemy()
{
}

void Enemy::update()
{
	if (name == "EasyEnemy") {//��� ��������� � ����� ������ ������ ����� �����
		
		gamer.setPosition(x, y);
		hb.setPosition(x - 10, y - 10);
		hb.setProgress((float)xp / (float)max_Xp);
		hb.setXp(xp, max_Xp); 

		controlEnemy();
		interactionWithMap(); //����� � ������
		go_move(); //�������� ����
		ai(); //��������� � ������
		
		if (xp <= 0) { life = false; gamer.setColor(Color::Black); }
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
	back = 0; //�����
	
	int nx = 0, ny = 0; //�����������
	if (x_move[0] > x) nx = 1; else nx = -1; //�� �������
	if (y_move[0] > y) ny = 1; else ny = -1;

	//���� �� ������� ������� num �� ���������� ����� 32 �� 32
	if (nx == 1) right = true; else left = true;
	if (ny == 1) back = true; else forward = true;

	//������� ���������� ��������
	if (x_move[0] + 1 == x) nx = 0;
	if (y_move[0] + 1 == y) ny = 0;
	if (nx == 0) { left = false; right = false; }
	if (ny == 0) { forward = false; back = false; }
}

void Enemy::ai()
{
	bool sight = false; //��������� ����������
	int i1 = 0, j1 = 0; //��������� �� ����
	int opX = 0, opY = 0; //���������� ����������
	
	//-----------------------�����������
	for (int j = 0; j < world->getSize(); j++)
		if (world->getEntity(j)->getPid() != pid && CheckGroup(j) &&
			world->getEntity(j)->getPos().x > x - visibility && world->getEntity(j)->getPos().x < x + visibility &&
			world->getEntity(j)->getPos().y > y - visibility && world->getEntity(j)->getPos().y < y + visibility)
		{ 
			sight = true;
			i1 = world->getEntity(j)->getPos().y;
			j1 = world->getEntity(j)->getPos().x;
			pidOp = world->getEntity(j)->getPid(); //��������� ������� ��������� ����� ����
			jOp = j;
			break;
		}

	if (tether) //��������� �������� � ���������
		if (x < Point[0] - DistantionPoint || x > Point[0] + DistantionPoint ||
			y < Point[1] - DistantionPoint || y > Point[1] + DistantionPoint) //�� ������ DistantionPoint �� �����
		{
			x_move[0] = Point[0]; 
			y_move[0] = Point[1]; 
			//xp = max_Xp; //��� ������� - �������������� �� ���� ������� �� �������
			moving = true;
			sight = false;
		}
	
	if (!sight) return; 
	if (BlockAi) return; //������� ���� �������� ���� saveDistantion ���������

	opX = j1; 
	opY = i1; //��������� ����������� ���������� ����������

	i1 = i1 - y;  //��������� ��������� ��������� ���������� � ����
	j1 = j1 - x + (Width / 2); //i - ��� y, j1 - �

	//������� ����� ������/���������� ����� ���������� DistanceFromOpponent? � ����� ����������?
	int xx = 0, yy = 0; 
	if (opX > x) xx = j1 - DistanceFromOpponent * 32; else xx = j1 + DistanceFromOpponent * 32; //����� �� ������ �� ����������� 
	if (opY > y) yy = i1 - DistanceFromOpponent * 32; else yy = i1 + DistanceFromOpponent * 32; //� ������

	//����������
	x_move[0] = x + xx;
	y_move[0] = y + yy;
	moving = true;  
}

void Enemy::controlEnemy()
{
	if (*time < controltime) return;
	controltime = *time + 0.01;
	
	//��������
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
	
	//������� ��������
	if (forward && left) //�����-����
		gamer.setTextureRect(IntRect(Width * 5, 0, Width, Height));
	if (forward && right) //�����-�����
		gamer.setTextureRect(IntRect(0, Height, Width, Height));
	if (back && left) //����-����
		gamer.setTextureRect(IntRect(Width * 3, 0, Width, Height));
	if (back && right) //����-�����
		gamer.setTextureRect(IntRect(Width * 1, 0, Width, Height));

}

void Enemy::interactionWithMap()
{
	if (!Map->interactionMap(x, y, Height, Width)) return; //���� ����������� ��� �������
	
	if (back)
		y -= speedConst;
	if (forward)
		y += speedConst;
	if (right)
		x -= speedConst;
	if (left)
		x += speedConst;
}

void Enemy::updateXp(int atck)
{
	xp -= atck; 
}

bool Enemy::CheckGroup(int n)
{
	if (group == NULL) return true; //���� ������ �� ��� �������
	
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
	
	Width = atoi(Person->Attribute("width")); //������� �������������
	Height = atoi(Person->Attribute("height"));
	xp = max_Xp = atoi(Person->Attribute("xp"));
	speed = atoi(Person->Attribute("speed"));
	visibility = atoi(Person->Attribute("visibility"));
	DistanceFromOpponent = atoi(Person->Attribute("DistanceFromOpponent"));
	
	//��������� ���������� � �����
	if (Person->FirstChildElement("AttackDamage")) { //������ ��������� ���� �������� �� ����� ����� ���������
		rations.at1 = atoi(Person->FirstChildElement("AttackDamage")->Attribute("At1"));
		rations.at2 = atoi(Person->FirstChildElement("AttackDamage")->Attribute("At2"));
		rations.at3 = atoi(Person->FirstChildElement("AttackDamage")->Attribute("At3"));
		rations.normAt = atoi(Person->FirstChildElement("AttackDamage")->Attribute("normAt"));
		rations.currentAt = rations.normAt;
	}

	//��������� SA
	TiXmlElement* SAElement = Person->FirstChildElement("SA"); //�������� ������������� ����

	for (int numb = 0; numb < 3; numb++) //3 SA
	{
		if (numb != 0)
			if (!(SAElement = SAElement->NextSiblingElement("SA"))) //���� SA ��� ������ �������
				return;

		//��������� ������ � string � ����� �� float
		TiXmlElement* DelayTix = SAElement->FirstChildElement("delay");
		TiXmlElement* durationTix = SAElement->FirstChildElement("duration");
		TiXmlElement* TileTix = SAElement->FirstChildElement("Tile");
		TiXmlElement* Description = SAElement->FirstChildElement("Description");

		string delayStr = DelayTix->Attribute("Value");
		string durationStr = durationTix->Attribute("Value");
		string TileX = TileTix->Attribute("ValueX");
		string TileY = TileTix->Attribute("ValueY");

		CurrentAttack SAAttack;
		//------------------------------------��������� string � ������
		interpretation inter;
		int Size = atoi(Description->Attribute("QuanityTile"));
		int iSize = Size * sizeof(int);
		int fSize = Size * sizeof(float);

		inter = delayStr;
		inter.toFloatMassv(&SAAttack.delay[0], fSize);

		inter = durationStr;
		inter.toFloatMassv(&SAAttack.duration[0], fSize);

		int uk[100]; //���� ���� ��������� ������ ��������� ��� �� ���

		inter = TileX;
		inter.toIntMassv(&uk[0], iSize);

		for (int j = 0; j < Size; j++)
			SAAttack.Tile[j][0] = uk[j];

		inter = TileY;
		inter.toIntMassv(&uk[0], iSize);

		for (int j = 0; j < Size; j++)
			SAAttack.Tile[j][1] = uk[j];
		//-----------------------------------------------------------
		//��������� �������������� ��������

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
		
		//������ VidE (���� ����)
		if (SAElement->FirstChildElement("mobs")) {
			TiXmlElement* VidE = SAElement->FirstChildElement("mobs");
			SAAttack.VidE = atoi(VidE->Attribute("Vide"));
			SAAttack.tVide = atoi(VidE->Attribute("tVide"));
			int qVide = atoi(VidE->Attribute("qVide")); 

			if (qVide != 0 && SAAttack.VidE != 0) //���� ����� ��������������
				for (int j = 0; j < atoi(VidE->Attribute("qVide")); j++) 
					AttackMode.addObj(AttackMode.getPr(), AttackMode.getSAAttack(numb));
		}
		AttackMode.setSAAttack(&SAAttack, numb);
	}
}

void Enemy::setGroup(Group_Rpg* group_)
{
	group = group_;
	group_->vecAS.push_back(&AttackMode); //��������� ����������� AttackMode

	AttackMode.getPr()->group = group;
	AttackMode.inisialization();
}

void Enemy::setTether(int DistantionPoint_)
{
	tether = true;
	Point[0] = x; //����� �������� ����� ����� ��������� �����������
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


