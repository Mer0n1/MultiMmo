#include "UniqueCharacters.h"

//---------------Chara

Chara::Chara() : Enemy("Chara", 96, 0), attack(AttackMode->getPr(), AttackMode->getSAAttack(0), AttackMode->getAnimation()->left)
{
	specialAt = false; 
	sleeptime = 0;
	arg = -1;
	RechargeSpecialAt = 10;
}

Chara::Chara(int IDO_) : Enemy("Chara", 96, IDO_), attack(AttackMode->getPr(), AttackMode->getSAAttack(0), AttackMode->getAnimation()->left)
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
	hb.setProgress((float)hp / (float)max_hp);
	hb.setHp(hp, max_hp);
	pidOp = 0;
	
	controlEnemy();
	interactionWithMap(); //связь с картой
	go_move(); //движение бота
	ai(); //двигаемся только если активно specialAt

	if (pidOp != 0) //если противник не найден не активируем систему атаки
		attacks();

	AttackMode->go_attack();

	if (hp <= 0) life = false;
}

void Chara::attacks()
{
	pr->posX = new float(attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() / 2); //сделаем отслеживание позиции противника
	pr->posY = new float(attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() / 2);

	attack::time = clock.getElapsedTime().asSeconds();
	//----------------------------------------
	
	//1 способность
	if (!AttackMode->getNum(3))
		AttackMode->setNum(1, true);

	//2 способность
	if (!AttackMode->getNum(1))
		AttackMode->setNum(2, true);

	//3 способность
	if (!AttackMode->getNum(2))
		AttackMode->setNum(3, true);
	//sp attack
	if (!AttackMode->getNum(3))
		specialAt = true;

	//------------------------------------
	if (AttackMode->getNum(1)) //запускаем атаку множеством обьектов (1 атака чары)
		for (int j = 0; j < AttackMode->getMany().size(); j++) {
			
			if (AttackMode->getMany()[j].getVidE() != 0)
				AttackMode->getMany()[j].go_attack(); //запуск рендера обьекта

			if (sleeptime < attack::time) { //очередь нового обьекта
				sleeptime = attack::time + 0.7;
				arg++;
			}

			if (arg == AttackMode->getMany().size() + 2) {
				AttackMode->setNum(1, false); arg = -1;
			} //отключаем атаку как только обьекты заканчиваются

			if (arg == j) break; //выходим чтобы соблюдать очередь обьектов 
		}

	//дело в скорости (слишком быстро) //дело еще в том, что атака не начинается пока другая идет //AttackMode
	if (arg <= AttackMode->getMany().size() && !AttackMode->getNum(1))
		AttackMode->setNum(1, true);
	
	//спец атака
	if (specialAt || life_attack) { //может быть переместить ее в attacks?
		special_attack();
		
		if (!life_attack) specialAt = false; else
		if (!BlockAi) //таким образом после того как будет задана точка нанесется атака
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
	if (attack::time_save > attack::time) return; //перезарядка навыка
	
	tiles[0].coord.x = *attack::pr->x - 32;
	tiles[0].coord.y = *attack::pr->y;
	tiles[1].coord.x = *attack::pr->x - 32;
	tiles[1].coord.y = *attack::pr->y + 32;
	tiles[2].coord.x = *attack::pr->x - 32;
	tiles[2].coord.y = *attack::pr->y + 64;

	int save = SAAttack->SizeTile;
	tiles[0].delay = 1;
	SAAttack->SizeTile = 32; 
	type_attack = Do;
	*pr->currentAt = rations.normAt; //задаем урон

	if (BlockVidE == 0) {
		life_attack = true;
		BlockVidE = 1;
	}
	
	attack_start();

	SAAttack->SizeTile = save;
	if (!life_attack) attack::time_save = attack::time + RechargeSpecialAt; //добавляем время перезярядки
	//go mov с увеличением скорости
	//удар может быть либо с правой либо с левой стороны
}


///////////////////////////////////////////////////////////////////////////


Papyrus::Papyrus(int IDO) : Enemy("Papyrus", 97, IDO), attack(AttackMode->getPr(), AttackMode->getSAAttack(0), AttackMode->getAnimation()->left)
{
	num3_ = 0;
	specialAt = 0;
	timerNum3 = 0;
	timerSpecialAt = 0;
	attack::inisialization();
}
Papyrus::Papyrus() : Enemy("Papyrus", 97, 0), attack(AttackMode->getPr(), AttackMode->getSAAttack(0), AttackMode->getAnimation()->left)
{
	num3_ = 0;
	specialAt = 0;
	timerNum3 = 0;
	timerSpecialAt = 0;
	attack::inisialization();
}

void Papyrus::update()
{
	gamer.setPosition(x, y);
	hb.setPosition(x - 10, y - 10);
	hb.setProgress((float)hp / (float)max_hp);
	hb.setHp(hp, max_hp);
	pidOp = 0; //сброс айди оппонента если 

	controlEnemy();
	interactionWithMap(); //связь с картой
	go_move(); //движение бота
	ai(); //поведение к игроку

	if (pidOp != 0) //если видит противника
		attacks();

	if (hp <= 0) life = false;
}

void Papyrus::special_attack()
{
	//замедление противника
	if (attack::time < timerSpecialAt) return;
	if (!life_attack) {
		attack::time = 0; clock.restart();
		timerSpecialAt = 0;
	} //нужен счетчик времени атаки

	life_attack = true;

	attack::world->getEntity(jOp)->setSpeed(attack::world->getEntity(jOp)->getSpeed() / 3); //замедляем
	attack::world->getEntity(jOp)->getModel()->setColor(Color(230, 146, 60));

	if (attack::time > 10) life_attack = false; //конец если замедление используется больше 10 сек
	if (!life_attack) { //конец атаки
		attack::world->getEntity(jOp)->getModel()->setColor(Color::White); //возвращаем цвет
		timerSpecialAt = attack::time + 20;
	}
}

void Papyrus::attacks()
{
	pr->posX = new float(attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() / 2); //сделаем отслеживание позиции противника
	pr->posY = new float(attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() / 2);

	attack::time = clock.getElapsedTime().asSeconds();
	//----------------------------------------------Условия запуса атак
	//1 способность (признаки)
	if (true) //условие - пока позволяет перезарядка
		AttackMode->setNum(1, true);

	//2 способности нет, итак слишком жесткий

	//3 способность
	if (true) { //условие - пока позволяет перезарядка
		num3_ = true;
		timerNum3 = attack::time + 10;
		pr->currentAt = pr->at3; //делаем текущую атаку 3
	}
	//----------

	if (*AttackMode->getPr()->hp != *AttackMode->getPr()->maxHp) //наступает когда его ранят
		special_attack(); //замедление от папируса

	// 3 атака (атака создает обьекты (множественная атака))
	if (num3_) //3 атака
	{
		//В данном случае мы не используем AttackSystem т.к нам нужны немного другие свойства и алгоритм атаки
		attack3();
		if (!life_attack) //атака обьекта закончилась - перезапускаем
			BlockVidE = 0;

		if (timerNum3 < attack::time && !life_attack) //таймер истек - атака закончилась
			num3_ = false;
	}

	AttackMode->go_attack();
}

void Papyrus::attack3()
{
	// создание вокруг пройтивника лучей
	if (BlockVidE == 0)
	{
		Vector2f end, begin;
		distantion = 30;
		
		end.x = attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() /2; //поиск и сохранение позиций противника
		end.y = attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() /2;
		//---------------------------
		int idx = 1;
		begin.x = rand() % 820 - 300; //установка координат относительно зоны игрока end точки
		
		if (begin.x < 0) idx = -1;
		begin.y = 520 - begin.x * idx;

		if (rand() % 2 == 1) begin.x *= -1;
		if (rand() % 2 == 0) begin.y *= -1; 
		
		begin.x = end.x + begin.x; //координаты begin переводим из относительных координат в мировые
		begin.y = end.y + begin.y;
		
		end.x   /= SAAttack->SizeTile;
		end.y   /= SAAttack->SizeTile;
		begin.x /= SAAttack->SizeTile;
		begin.y /= SAAttack->SizeTile;
		//---------------------------

		Vector2i coord[20];
		for (int j = 0; j < 20; j++)
		{
			coord[j].x = tiles[j].coord.x;
			coord[j].y = tiles[j].coord.y;
		}
		algorithmLine(begin, end, &coord[0]);

		for (int j = 0; j < 20; j++)
		{
			tiles[j].coord.x = coord[j].x * SAAttack->SizeTile;
			tiles[j].coord.y = coord[j].y * SAAttack->SizeTile; 
		}
		
		tiles[0].delay = 1;
		tiles[18].duration = 1;
		type_attack = Do;
		SAAttack->SizeTile = 32; //может быть ошибка
		life_attack = true;
		BlockVidE = 1; 
	}
	attack_start();
}

//////////////////////////////////////////////////////////////

Sans::Sans(int IDO) : Enemy("Sans", 98, IDO), attack(AttackMode->getPr(), AttackMode->getSAAttack(0), AttackMode->getAnimation()->left)
{
	teleport_ = false;
	num4 = false;
	DistanceFromOpponent = 7;
	RechargeTeleport = 0;
	DelaySpAt = 0;
}
Sans::Sans() : Enemy("Sans", 98, 0), attack(AttackMode->getPr(), AttackMode->getSAAttack(0), AttackMode->getAnimation()->left)
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
	hb.setProgress((float)hp / (float)max_hp);
	hb.setHp(hp, max_hp);

	controlEnemy();
	interactionWithMap(); //связь с картой
	go_move(); //движение бота
	ai(); //поведение к игроку

	attacks(); //Pid у Санса фиксируется даже когда он потерял из виду врага
	AttackMode->go_attack();

	if (hp <= 0) life = false;
}

void Sans::special_attack()
{
	//особая способность санса изменение координат противника
	if (!life_attack) { //атака только начинается
		clock_Sp.restart(); //нужен счетчик времени атаки
		DelaySpAt = 0;
		BlockVidE = 0;
		life_attack = true;
	}
	
	TimerSp = clock_Sp.getElapsedTime().asSeconds() + 1;
	if (TimerSp < DelaySpAt) return; //регулирование скорости задержкой

	if (BlockVidE == 0) //находим начальную и конечную точки
	{
		posX_VidE = attack::world->getEntity(jOp)->getPos().x + 500; //конечные точки
		posY_VidE = attack::world->getEntity(jOp)->getPos().y + 400;
		
		saveX_VidE = attack::world->getEntity(jOp)->getPos().x - attack::world->getEntity(jOp)->getWidth() / 2 - posX_VidE; //дистанция от начала до конца
		saveY_VidE = attack::world->getEntity(jOp)->getPos().y - attack::world->getEntity(jOp)->getHeight() / 2 - posY_VidE;

		BlockVidE = 1;
	}
	
	//алгоритм построение отрезка (в данном случае мы не используем algorithmLine т.к нам нужны другие данные)
	float poX = saveX_VidE; //переведем во float
	float poY = saveY_VidE;
	int nx = 0, ny = 0;
	float dx = 0, dy = 0; //переменные (на сколько суммировать позицию? чтобы дойти до конца отрезка)

	if (saveX_VidE + posX_VidE < posX_VidE) nx = 1; else nx = -1; //по графику
	if (saveY_VidE + posY_VidE < posY_VidE) ny = 1; else ny = -1;
	
	if (poX / poY > 1 || poX / poY < -1) { //если x превышает 1 то x = 1 а y < 1

		dx = nx; //высчитываем алгоритм прибавления для равномерной уменьшении дистанции
		dy = poY / poX * nx;

	} else { //наоборот
		dx = poX / poY * ny;
		dy = ny;
	}
	/////////////////////////////////////////
	//график скорости
	DelaySpAt = TimerSp + 0.01;
	int kof = pow(TimerSp, 6) * 4/*скорость*/;
	
	if (TimerSp < 1.1) {
		dx *= -1;
		dy *= -1;
	} //обратный ход

	dx *= kof;
	dy *= kof; 
	//////////////////////////////////////////
	
	if (attack::world->getEntity(jOp)->getPos().x >= posX_VidE &&
		attack::world->getEntity(jOp)->getPos().y >= posY_VidE)
		life_attack = false; //останавливаем при достижении точки конца 

 	attack::world->getEntity(jOp)->addPos(dx, dy); //изменяем координаты
	attack::world->getEntity(jOp)->getModel()->setColor(Color::Blue);

	if (!life_attack) { //конец атаки
		attack::world->getEntity(jOp)->getModel()->setColor(Color::White); //возвращаем цвет
		attack::world->getEntity(jOp)->updateHp(10); //урон
		DelaySpAt = TimerSp + 5;
	}
}

void Sans::attacks()
{
	bool BlockAi = false;
	bool SPNum4 = false; //переменная активирующая спец атаку санса

	pr->posX = new float(attack::world->getEntity(jOp)->getPos().x + attack::world->getEntity(jOp)->getWidth() / 2); //сделаем отслеживание позиции противника
	pr->posY = new float(attack::world->getEntity(jOp)->getPos().y + attack::world->getEntity(jOp)->getHeight() / 2);

	attack::time = clock.getElapsedTime().asSeconds(); //таймер не имеет никакого отношения к AttackMode
	//-----------------------------------------------------------------
	
	//Признаки и следствия активации телепорта и спец атаки
	if (hp < max_hp)
	if (*AttackMode->getPr()->x - attack::world->getEntity(jOp)->getPos().x > 1500 ||
		*AttackMode->getPr()->x - attack::world->getEntity(jOp)->getPos().x < -1500 ||
		*AttackMode->getPr()->y - attack::world->getEntity(jOp)->getPos().y > 1500 ||
		*AttackMode->getPr()->y - attack::world->getEntity(jOp)->getPos().y < -1500)
		teleport_ = true; //телепорт если противник далеко

	if (attack::world->getEntity(jOp)->getPos().x > x - 200 &&
		attack::world->getEntity(jOp)->getPos().x < x + 200 &&
		attack::world->getEntity(jOp)->getPos().y > y - 200 &&
		attack::world->getEntity(jOp)->getPos().y < y + 200)
		SPNum4 = true; //если близко к себе отталкивание
	
	//--------------------------------
	//1 атака
	if ((!SPNum4 && AttackMode->getNum(2)) || //если SPNum4(спец.атака) не активна 
		(attack::world->getEntity(jOp)->getHp() < attack::world->getEntity(jOp)->getMaxHp() / 10)) //если у противника меньше 10% хп (добить)
			AttackMode->setNum(1, true);

	//2 атака
	if ((attack::world->getEntity(jOp)->getPos().x < x - 200 && attack::world->getEntity(jOp)->getPos().x > x - 600) ||
		(attack::world->getEntity(jOp)->getPos().x > x + 200 && attack::world->getEntity(jOp)->getPos().x < x + 600) ||
		(attack::world->getEntity(jOp)->getPos().y < y - 200 && attack::world->getEntity(jOp)->getPos().y > y - 600) ||
		(attack::world->getEntity(jOp)->getPos().y > y + 200 && attack::world->getEntity(jOp)->getPos().y < y + 600)) //средняя дистанция
		AttackMode->setNum(2, true);

	//3 атака
	if (teleport_ ||
		attack::world->getEntity(jOp)->getHp() < attack::world->getEntity(jOp)->getMaxHp() / 4) //если у противника меньше 25% хп (добить)
		AttackMode->setNum(3, true);
	//---------------------------------------

	if (teleport_ && attack::time > RechargeTeleport) //использование отброса после телепорта
		if (rand() & 10 + 1 > 7) SPNum4 = true; //с шансом 30%

	if (teleport_) { //телепорт санса
		teleport();
		teleport_ = false; 
		BlockAi = true;
	}
	
	if (life_attack || SPNum4) {
		special_attack(); //спец атака 
		teleport_ = false;
		SPNum4 = false;
	}
}

void Sans::teleport()
{
	if (RechargeTeleport < attack::time) BlockVidE = 0; //перезарядка навыка телепорта
	if (BlockVidE != 0) return;
	RechargeTeleport = attack::time + 2; //таймер для перезарядки
	
	//Изменение своих координат
	x = attack::world->getEntity(jOp)->getPos().x + rand() % 1000 - 500;
	y = attack::world->getEntity(jOp)->getPos().y + rand() % 1000 - 500;

	BlockVidE = 1;
}