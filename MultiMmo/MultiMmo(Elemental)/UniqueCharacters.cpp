#include "UniqueCharacters.h"

//---------------Chara

Chara::Chara() : Enemy("Chara", 96, 0), attack(AttackMode.getPr(), AttackMode.getSAAttack(0), AttackMode.getAnimation()->left)
{
	specialAt = false; 
	sleeptime = 0;
	arg = -1;
	RechargeSpecialAt = 10;
}

Chara::Chara(int IDO_) : Enemy("Chara", 96, IDO_), attack(AttackMode.getPr(), AttackMode.getSAAttack(0), AttackMode.getAnimation()->left)
{
	specialAt = false;
	sleeptime = 0;
	arg = -1; 
	RechargeSpecialAt = 10;
}

void Chara::update()
{
	gamer.setPosition(x, y);
	hb.setPosition(x - 10, y - 10);
	hb.setProgress((float)xp / (float)max_Xp);
	hb.setXp(xp, max_Xp);
	pidOp = 0;
	
	controlEnemy();
	interactionWithMap(); //����� � ������
	go_move(); //�������� ����
	ai(); //��������� ������ ���� ������� specialAt

	if (pidOp != 0) //���� ��������� �� ������ �� ���������� ������� �����
		attacks();

	AttackMode.go_attack();

	if (xp <= 0) life = false;
}

void Chara::attacks()
{
	pr->posX = new float(attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() / 2); //������� ������������ ������� ����������
	pr->posY = new float(attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() / 2);

	attack::time = clock.getElapsedTime().asSeconds();
	//----------------------------------------
	
	//1 �����������
	if (!AttackMode.getNum(3))
		AttackMode.setNum(1, true);

	//2 �����������
	if (!AttackMode.getNum(1)) 
		AttackMode.setNum(2, true);

	//3 �����������
	if (!AttackMode.getNum(2)) 
		AttackMode.setNum(3, true);
	//sp attack
	if (!AttackMode.getNum(3))
		specialAt = true;

	//------------------------------------
	if (AttackMode.getNum(1)) //��������� ����� ���������� �������� (1 ����� ����)
		for (int j = 0; j < AttackMode.getMany().size(); j++) {
			
			if (AttackMode.getMany()[j].getVidE() != 0)
				AttackMode.getMany()[j].go_attack(); //������ ������� �������

			if (sleeptime < attack::time) { //������� ������ �������
				sleeptime = attack::time + 0.7;
				arg++;
			}

			if (arg == AttackMode.getMany().size() + 2) {
				AttackMode.setNum(1, false); arg = -1;
			} //��������� ����� ��� ������ ������� �������������

			if (arg == j) break; //������� ����� ��������� ������� �������� 
		}

	//���� � �������� (������� ������) //���� ��� � ���, ��� ����� �� ���������� ���� ������ ���� //AttackMode
	if (arg <= AttackMode.getMany().size() && !AttackMode.getNum(1))
		AttackMode.setNum(1, true);
	
	//���� �����
	if (specialAt || life_attack) { //����� ���� ����������� �� � attacks?
		special_attack();
		
		if (!life_attack) specialAt = false; else
		if (!BlockAi) //����� ������� ����� ���� ��� ����� ������ ����� ��������� �����
		{
			x_move[0] = Enemy::world->getEntity(jOp)->getPos().x + 60;
			y_move[0] = Enemy::world->getEntity(jOp)->getPos().y;
			moving = true;
		}
		BlockAi = specialAt;
	}
}

void Chara::special_attack()
{	
	if (!life_attack) BlockVidE = 0;
	if (attack::time_save > attack::time) return; //����������� ������

	x_attack[0] = *attack::pr->x - 32;
	y_attack[0] = *attack::pr->y;
	x_attack[1] = *attack::pr->x - 32;
	y_attack[1] = *attack::pr->y + 32;
	x_attack[2] = *attack::pr->x - 32;
	y_attack[2] = *attack::pr->y + 64;

	int save = SAAttack->SizeTile;
	delay_attack[0] = 1;
	SAAttack->SizeTile = 32; 
	type_attack = Do;
	*pr->currentAt = rations.normAt; //������ ����

	if (BlockVidE == 0) {
		life_attack = true;
		BlockVidE = 1;
	}
	
	attack_start();

	SAAttack->SizeTile = save;
	if (!life_attack) attack::time_save = attack::time + RechargeSpecialAt; //��������� ����� �����������
	//go mov � ����������� ��������
	//���� ����� ���� ���� � ������ ���� � ����� �������
}


///////////////////////////////////////////////////////////////////////////


Papyrus::Papyrus(int IDO) : Enemy("Papyrus", 97, IDO), attack(AttackMode.getPr(), AttackMode.getSAAttack(0), AttackMode.getAnimation()->left)
{
	num3_ = 0;
	specialAt = 0;
	timerNum3 = 0;
	timerSpecialAt = 0;
}
Papyrus::Papyrus() : Enemy("Papyrus", 97, 0), attack(AttackMode.getPr(), AttackMode.getSAAttack(0), AttackMode.getAnimation()->left)
{
	num3_ = 0;
	specialAt = 0;
	timerNum3 = 0;
	timerSpecialAt = 0;
}

void Papyrus::update()
{
	gamer.setPosition(x, y);
	hb.setPosition(x - 10, y - 10);
	hb.setProgress((float)xp / (float)max_Xp);
	hb.setXp(xp, max_Xp);
	pidOp = 0; //����� ���� ��������� ���� 

	controlEnemy();
	interactionWithMap(); //����� � ������
	go_move(); //�������� ����
	ai(); //��������� � ������

	if (pidOp != 0) //���� ����� ����������
		attacks();

	if (xp <= 0) life = false;
}

void Papyrus::special_attack()
{
	//���������� ����������
	if (attack::time < timerSpecialAt) return;
	if (!life_attack) {
		attack::time = 0; clock.restart();
		timerSpecialAt = 0;
	} //����� ������� ������� �����

	life_attack = true;

	attack::world->getEntity(jOp)->setSpeed(attack::world->getEntity(jOp)->getSpeed() / 3); //���������
	attack::world->getEntity(jOp)->getModel()->setColor(Color(230, 146, 60));

	if (attack::time > 10) life_attack = false; //����� ���� ���������� ������������ ������ 10 ���
	if (!life_attack) { //����� �����
		attack::world->getEntity(jOp)->getModel()->setColor(Color::White); //���������� ����
		timerSpecialAt = attack::time + 20;
	}
}

void Papyrus::attacks()
{
	pr->posX = new float(attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() / 2); //������� ������������ ������� ����������
	pr->posY = new float(attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() / 2);

	attack::time = clock.getElapsedTime().asSeconds();
	//----------------------------------------------������� ������ ����
	//1 ����������� (��������)
	if (true) //������� - ���� ��������� �����������
		AttackMode.setNum(1, true);

	//2 ����������� ���, ���� ������� �������

	//3 �����������
	if (true) { //������� - ���� ��������� �����������
		num3_ = true;
		timerNum3 = attack::time + 10;
		pr->currentAt = pr->at3; //������ ������� ����� 3
	}
	//----------

	if (*AttackMode.getPr()->xp != *AttackMode.getPr()->maxXp) //��������� ����� ��� �����
		special_attack(); //���������� �� ��������

	// 3 ����� (����� ������� ������� (������������� �����))
	if (num3_) //3 �����
	{
		//� ������ ������ �� �� ���������� AttackSystem �.� ��� ����� ������� ������ �������� � �������� �����
		attack3();
		if (!life_attack) //����� ������� ����������� - �������������
			BlockVidE = 0;

		if (timerNum3 < attack::time && !life_attack) //������ ����� - ����� �����������
			num3_ = false;
	}

	AttackMode.go_attack();
}

void Papyrus::attack3()
{
	// �������� ������ ����������� �����
	if (BlockVidE == 0)
	{
		Vector2f end, begin;
		distantion = 30;
		
		end.x = attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() / 2; //����� � ���������� ������� ����������
		end.y = attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() / 2;
		//---------------------------
		int idx = 1;
		begin.x = rand() % 820 - 300; //��������� ��������� ������������ ���� ������ end �����
		
		if (begin.x < 0) idx = -1;
		begin.y = 520 - begin.x * idx;

		if (rand() % 2 == 1) begin.x *= -1;
		if (rand() % 2 == 0) begin.y *= -1; 
		
		begin.x = end.x + begin.x; //���������� begin ��������� �� ������������� ��������� � �������
		begin.y = end.y + begin.y;
		
		end.x /= SAAttack->SizeTile;
		end.y /= SAAttack->SizeTile;
		begin.x /= SAAttack->SizeTile;
		begin.y /= SAAttack->SizeTile;
		//---------------------------
		
		Vector2f* ms = algorithmLine(begin, end);  //������������ ����� - ����������� ���� ������ � 20
		for (int j = 0; j < 20; j++) { //������� � ������������ ������
			x_attack[j] = ms->x;
			y_attack[j] = ms->y;
			x_attack[j] *= SAAttack->SizeTile;
			y_attack[j] *= SAAttack->SizeTile;
			ms++;
		}

		delay_attack[0] = 1;
		duration_attack[18] = 1;
		type_attack = Do;
		SAAttack->SizeTile = 32; //����� ���� ������
		life_attack = true;
		BlockVidE = 1;
	}
	attack_start();
}

//////////////////////////////////////////////////////////////

Sans::Sans(int IDO) : Enemy("Sans", 98, IDO), attack(AttackMode.getPr(), AttackMode.getSAAttack(0), AttackMode.getAnimation()->left)
{
	teleport_ = false;
	num4 = false;
	DistanceFromOpponent = 7;
	RechargeTeleport = 0;
	DelaySpAt = 0;
}
Sans::Sans() : Enemy("Sans", 98, 0), attack(AttackMode.getPr(), AttackMode.getSAAttack(0), AttackMode.getAnimation()->left)
{
	teleport_ = false;
	num4 = false;
	DistanceFromOpponent = 7;
	RechargeTeleport = 0;
	DelaySpAt = 0;
}

void Sans::update()
{
	gamer.setPosition(x, y);
	hb.setPosition(x - 10, y - 10);
	hb.setProgress((float)xp / (float)max_Xp);
	hb.setXp(xp, max_Xp);

	controlEnemy();
	interactionWithMap(); //����� � ������
	go_move(); //�������� ����
	ai(); //��������� � ������

	attacks(); //Pid � ����� ����������� ���� ����� �� ������� �� ���� �����
	AttackMode.go_attack();

	if (xp <= 0) life = false;
}

void Sans::special_attack()
{
	//������ ����������� ����� ��������� ��������� ����������
	if (!life_attack) { //����� ������ ����������
		clock_Sp.restart(); //����� ������� ������� �����
		DelaySpAt = 0;
		BlockVidE = 0;
		life_attack = true;
	}
	
	TimerSp = clock_Sp.getElapsedTime().asSeconds() + 1;
	if (TimerSp < DelaySpAt) return; //������������� �������� ���������

	if (BlockVidE == 0) //������� ��������� � �������� �����
	{
		posX_VidE = attack::world->getEntity(jOp)->getPos().x + 500; //�������� �����
		posY_VidE = attack::world->getEntity(jOp)->getPos().y + 400;
		
		saveX_VidE = attack::world->getEntity(jOp)->getPos().x - attack::world->getEntity(jOp)->getWidth() / 2 - posX_VidE; //��������� �� ������ �� �����
		saveY_VidE = attack::world->getEntity(jOp)->getPos().y - attack::world->getEntity(jOp)->getHeight() / 2 - posY_VidE;

		BlockVidE = 1;
	}
	
	//�������� ���������� ������� (� ������ ������ �� �� ���������� algorithmLine �.� ��� ����� ������ ������)
	float poX = saveX_VidE; //��������� �� float
	float poY = saveY_VidE;
	int nx = 0, ny = 0;
	float dx = 0, dy = 0; //���������� (�� ������� ����������� �������? ����� ����� �� ����� �������)

	if (saveX_VidE + posX_VidE < posX_VidE) nx = 1; else nx = -1; //�� �������
	if (saveY_VidE + posY_VidE < posY_VidE) ny = 1; else ny = -1;
	
	if (poX / poY > 1 || poX / poY < -1) { //���� x ��������� 1 �� x = 1 � y < 1

		dx = nx; //����������� �������� ����������� ��� ����������� ���������� ���������
		dy = poY / poX * nx;

	} else { //��������
		dx = poX / poY * ny;
		dy = ny;
	}
	/////////////////////////////////////////
	//������ ��������
	DelaySpAt = TimerSp + 0.01;
	int kof = pow(TimerSp, 6) * 4/*��������*/;
	
	if (TimerSp < 1.1) {
		dx *= -1;
		dy *= -1;
	} //�������� ���

	dx *= kof;
	dy *= kof; 
	//////////////////////////////////////////
	
	if (attack::world->getEntity(jOp)->getPos().x >= posX_VidE &&
		attack::world->getEntity(jOp)->getPos().y >= posY_VidE)
		life_attack = false; //������������� ��� ���������� ����� ����� 

 	attack::world->getEntity(jOp)->addPos(dx, dy); //�������� ����������
	attack::world->getEntity(jOp)->getModel()->setColor(Color::Blue);

	if (!life_attack) { //����� �����
		attack::world->getEntity(jOp)->getModel()->setColor(Color::White); //���������� ����
		attack::world->getEntity(jOp)->updateXp(10); //����
		DelaySpAt = TimerSp + 5;
	}
}

void Sans::attacks()
{
	bool BlockAi = false;
	bool SPNum4 = false; //���������� ������������ ���� ����� �����

	pr->posX = new float(attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() / 2); //������� ������������ ������� ����������
	pr->posY = new float(attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() / 2);

	attack::time = clock.getElapsedTime().asSeconds(); //������ �� ����� �������� ��������� � AttackMode
	//-----------------------------------------------------------------
	
	//�������� � ��������� ��������� ��������� � ���� �����
	if (xp < max_Xp)
	if (*AttackMode.getPr()->x - attack::world->getEntity(jOp)->getPos().x > 1500 ||
		*AttackMode.getPr()->x - attack::world->getEntity(jOp)->getPos().x < -1500 ||
		*AttackMode.getPr()->y - attack::world->getEntity(jOp)->getPos().y > 1500 ||
		*AttackMode.getPr()->y - attack::world->getEntity(jOp)->getPos().y < -1500)
		teleport_ = true; //�������� ���� ��������� ������

	if (attack::world->getEntity(jOp)->getPos().x > x - 200 &&
		attack::world->getEntity(jOp)->getPos().x < x + 200 &&
		attack::world->getEntity(jOp)->getPos().y > y - 200 &&
		attack::world->getEntity(jOp)->getPos().y < y + 200)
		SPNum4 = true; //���� ������ � ���� ������������
	
	//--------------------------------
	//1 �����
	if ((!SPNum4 && AttackMode.getNum(2)) || //���� SPNum4(����.�����) �� ������� 
		(attack::world->getEntity(jOp)->getXp() < attack::world->getEntity(jOp)->getMaxXp() / 10)) //���� � ���������� ������ 10% �� (������)
			AttackMode.setNum(1, true);

	//2 �����
	if ((attack::world->getEntity(jOp)->getPos().x < x - 200 && attack::world->getEntity(jOp)->getPos().x > x - 600) ||
		(attack::world->getEntity(jOp)->getPos().x > x + 200 && attack::world->getEntity(jOp)->getPos().x < x + 600) ||
		(attack::world->getEntity(jOp)->getPos().y < y - 200 && attack::world->getEntity(jOp)->getPos().y > y - 600) ||
		(attack::world->getEntity(jOp)->getPos().y > y + 200 && attack::world->getEntity(jOp)->getPos().y < y + 600)) //������� ���������
		AttackMode.setNum(2, true); 

	//3 �����
	if (teleport_ ||
		attack::world->getEntity(jOp)->getXp() < attack::world->getEntity(jOp)->getMaxXp() / 4) //���� � ���������� ������ 25% �� (������)
		AttackMode.setNum(3, true);
	//---------------------------------------

	if (teleport_ && attack::time > RechargeTeleport) //������������� ������� ����� ���������
		if (rand() & 10 + 1 > 7) SPNum4 = true; //� ������ 30%

	if (teleport_) { //�������� �����
		teleport();
		teleport_ = false; 
		BlockAi = true;
	}
	
	if (life_attack || SPNum4) {
		special_attack(); //���� ����� 
		teleport_ = false;
		SPNum4 = false;
	}
}

void Sans::teleport()
{
	if (RechargeTeleport < attack::time) BlockVidE = 0; //����������� ������ ���������
	if (BlockVidE != 0) return;
	RechargeTeleport = attack::time + 2; //������ ��� �����������
	
	//��������� ����� ���������
	x = attack::world->getEntity(jOp)->getPos().x + rand() % 1000 - 500;
	y = attack::world->getEntity(jOp)->getPos().y + rand() % 1000 - 500;

	BlockVidE = 1;
}