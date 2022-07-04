#include "Attack.h"
#include "GameWorld.h"
#include "windows.h"
//-----------------------------attacks
attack::attack() 
{
	
}

attack::attack(playerPr* pr_, CurrentAttack* SAAttack_, AnimationAttack* animation_)
{
	pr = pr_;
	SAAttack = SAAttack_;
	animation = animation_;
	
	for (int j = 0; j < 20; j++) id_opponents[j] = 0;
	for (int j = 0; j < 20; j++) tiles.push_back(Tile());

	clock.restart();
	time = 0;
	time_attack = 0;
	accounting = 0;
	newtimer = 0;
	save_j = -1;
	save_j2 = -1;
	saveS = 0;
	QuanTile = 20;
	distantion = 5;
	VidE = 0;
	saveX_VidE = 0;
	saveY_VidE = 0;
	posX_VidE = 0; 
	posY_VidE = 0;
	BlockVidE = 0; 
	timeAttack_VidE = 0; 
	VidE = SAAttack->VidE;
}

void attack::go_attack()
{
	time = clock.getElapsedTime().asSeconds();
	
	//проверка перезарядки и активности
	if (time > time_attack && !life_attack) {
		clock.restart(); //сброс счетчика (атака прекращается на момент когда life_attack не активна и перезарядка прошла)
		time = 0;
		time_attack = SAAttack->recharge; //перезарядка
		life_attack = true;
		animation->isActive = true;
	
		if (FuncAttack != "ClickPlace") //если не ClickPlace то фиксируем относительно игрока
			for (int j = 0; j < QuanTile; j++) {
				if (SAAttack->Tile[j][0] == 0 & SAAttack->Tile[j][1] == 0) continue;
				tiles[j].coord.x = (SAAttack->Tile[j][0] + *pr->x / tiles[j].size) * tiles[j].size;
				tiles[j].coord.y = (SAAttack->Tile[j][1] + *pr->y / tiles[j].size) * tiles[j].size;
			}
		if (type_attack == Do)
			if (VidE == 0) tiles[QuanTile - 2].duration = 0.2; //чтоб мгновенная атака хотябы была видна
	}
	
	//запуск атаки
	if (life_attack) attack_start();
}

void attack::attack_start()
{
	if (!life_attack) return; 
	else animation->isActive = true;
	funcAttack(); 
	
	for (int j = 0; j < 20; j++)
		id_opponents[j] = 1; //фиксирование всех айди по которым нанесется урон

	int CurrentAtt = *pr->currentAt; //текущий урон атаки
	active = true;
	int newJ = 0;

	for (int j = 0; j < QuanTile; j++) {

		if (j == QuanTile - 1) { //сброс
			animation->Reset();
			animation->isActive = false;
			saveX_VidE = 0; posX_VidE = 0;
			saveY_VidE = 0; posY_VidE = 0;
			BlockOnce = false;
			BlockVidE = 0;
			accounting = 0;
			newtimer = 0;
			save_j = -1;
			saveS = 0;
			//id_opponents[j] = 1;
			delayBlock = false;
			life_attack = false;
			break;
		}
		//---------------------------------------- Duration - delay

		if (save_j != -1 && !delayBlock) //делаем тайлы прозрачными когда действуте delay - задержка
			for (int c = 0; c < QuanTile; c++) {
				animation->intTile_8[c].setScale(tiles[c].size / 8.f, tiles[c].size / 8.f); //8
				animation->intTile_8[c].setColor(Color(255, 255, 255, 50)); //делаем полупрозрачным
				animation->intTile_8[c].setPosition(tiles[c].coord.x, tiles[c].coord.y); //изменение размера клеток в соотвествии с атакой
			}

		if (!delayBlock) { //если есть задержка
			if (tiles[j].delay != 0 & save_j < j) {
				newtimer = tiles[j].delay + time;
				save_j = j;
			} //фиксируем задержку		

			if (newtimer < time) {
				if (newtimer != 0) {
					j = save_j;
					newtimer = 0;
					save_j = -1;
				}
			}
			else break;
		}

		if (tiles[j].duration != 0 & save_j < j & time > newtimer) {
			newtimer = tiles[j].duration + time;
			save_j = j;
			delayBlock = true;
		} //фиксируем продолжительность	

		if (newtimer > time) { //продолжаем
			if (save_j2 == save_j)
				save_j2 = 1;
		}
		else if (save_j > -1) { //когда заканчивается продолжительность сбрасываем
			j = save_j; //возвращаем значение
			save_j = -1;
			newtimer = 0;
			delayBlock = false;
		}

		//---------------------------------------- //проверка интерактивности 

		for (int c = 0; c < world->getSize(); c++)
		{
			newJ = j;
			if (save_j != -1) newJ = save_j; //проходимся по всем тайлам который запоминает duration

			for (int jj = 0; jj < newJ; jj++)
				if (world->getEntity(c)->getPid() != pr->pid && //по себе урон не наносим
					tiles[jj].coord.x + tiles[jj].size >= world->getEntity(c)->getPos().x &&
					tiles[jj].coord.x <= world->getEntity(c)->getPos().x + world->getEntity(c)->getWidth() &&
					//проверяем попадает ли квадрат координат атаки по персонажу
					tiles[jj].coord.y <= world->getEntity(c)->getPos().y + world->getEntity(c)->getHeight() &&
					tiles[jj].coord.y + tiles[jj].size >= world->getEntity(c)->getPos().y)
				{
					if (!CheckGroup(pr, world->getEntity(c)->getId())) continue;
					for (int i = 0; i < 20; i++) {
						if (id_opponents[i] == world->getEntity(c)->getPid()) break; //если такой есть не добавляем

						if (id_opponents[i] == 1) { //фиксируем всех противников под атакой
							id_opponents[i] = world->getEntity(c)->getPid(); break;
						}
					}
					break;
				}
		}

		//---------------------------------------- Тип атаки
		if (type_attack != Do) accounting = 0; //если это не Do то продолжим циклом наносить урон

		if (type_attack == dpsa) //урон в секунду 
		{
			if (newtimer != 0) {
				active = false;
				if (time > saveS) {
					saveS = time + 1;
					if (VidE == 0) CurrentAtt = (*pr->currentAt) / tiles[save_j].duration;
					if (VidE != 0) CurrentAtt = (*pr->currentAt) / SAAttack->tVide;
					if (*pr->currentAt < 1) *pr->currentAt = 1; //не меньше 1
					active = true;
				}
			}
			else active = false;
		}

		if (type_attack == ida) //урон в мгновенье
		{
			if (newtimer != 0) {
				active = false;
				if (time > saveS) {
					if (VidE == 0) saveS = time + 1.f / (*pr->currentAt / tiles[save_j].duration);
					if (VidE != 0) saveS = time + 1.f / (*pr->currentAt / SAAttack->tVide);
					CurrentAtt = 1; active = true;
				}
			}
			else active = false;
		}

		//вид атаки 
		if (VidE != 0 && j == QuanTile - 2 && save_j == -1)  //если duration активно запрещаем
			if (timeAttack_VidE > time) { //остановка атаки по истечения времени
				j = 0; vide(); //каждый цикл - 1 кадр
				if (tiles[QuanTile - 2].duration == 0)
					tiles[QuanTile - 2].duration = 0.01;
			}
		//--------------------------------------------------- Нанесение урона

		if (active) //для блокировки атаки
			for (int c = 0; c < world->getSize(); c++)
				for (int i = accounting; i < 20; i++)
					if (world->getEntity(c)->getPid() == id_opponents[i]) {
						world->getEntity(c)->updateHp(CurrentAtt); //наносим урон
						client_br->MakeDamage(id_opponents[i], CurrentAtt); //отправляем урон запросом на сервер
						accounting = i + 1; //для Do
					}


		//анимация атаки
		if (tiles[j].coord.x != 0 && tiles[j].coord.y != 0) {
			animation->intTile_8[j].setColor(Color(255, 255, 255, 255)); //убираем прозрачность
			animation->intTile_8[j].setScale(tiles[j].size / 8.f, tiles[j].size / 8.f); //8
			animation->intTile_8[j].setPosition(tiles[j].coord.x, tiles[j].coord.y); //изменение размера клеток в соотвествии с атакой
		}

		if (newtimer > time) break; //выходим чтобы бесконечный цикл не создавать при продолжительности
	}

	
}

void attack::funcAttack()
{
	time = clock.getElapsedTime().asSeconds();
	
	if (Left || FuncAttack == "Left") //обычная атака
	{
		if (time > time_save) {

			if (pr->id == 99) {

				time_save = time + 0.5;
				int y1 = *pr->y / 32,
					x1 = *pr->x / 32;

				//найдем направление атаки
				float rotation = (atan2(*pr->posX - view->getCenter().x, *pr->posY - view->getCenter().y)) * 180 / 3.14;
				
				int y2 = y1, x2 = x1;
				if (rotation >= 160 || rotation <= -160) { y2 -= 2; x2 += 1; }
				else
					if (rotation >= 110 || rotation <= -110) { y2 -= 2; x2 -= 2; }
					else
						if (rotation >= 70 || rotation <= -70) { y2 += 1; x2 -= 2; }
						else
							if (rotation >= 50 || rotation <= -50) { y2 += 2; x2 -= 2; }
							else
								if (rotation >= 15 || rotation <= -15) { y2 += 5; x2 += 1; }
				if (rotation > 0) { x2 = x1 - x2 + x1 + 3; }
				
				type_attack = Do;
				tiles[0].coord.y = y2 * 32;
				tiles[0].coord.x = x2 * 32;
				SAAttack->SizeTile = 32; 
				life_attack = true;
			}
		}
		else Left = false;
	}

	if (FuncAttack == "line" && life_attack && !BlockOnce) //атака линией (тип - лазер)
	{
		int y1 = (*pr->y + pr->Height / 2) / SAAttack->SizeTile, //наша позиция
			x1 = (*pr->x + pr->Width / 2) / SAAttack->SizeTile;
		Vector2i coord[20]; //используем coord вместо tiles[].coord чтобы не путалисть адреса памяти

		algorithmLine(Vector2f(x1, y1), Vector2f(*pr->posX / SAAttack->SizeTile, *pr->posY / SAAttack->SizeTile), &coord[0]);
		for (int j = 0; j < QuanTile; j++) {
			tiles[j].coord.x = coord[j].x * tiles[j].size;
			tiles[j].coord.y = coord[j].y * tiles[j].size;
		}
	}

	if (FuncAttack == "ClickPlace" && life_attack && !BlockOnce) //функция атаки: по месту клика
	{
		//учитываем дистанцию
		int x1 = *pr->x + pr->Width / 2;
		int y1 = *pr->y + pr->Height / 2; 
		int S = 32; 
		
		if (*pr->posX - x1 > distantion * S) *pr->posX = x1 + distantion * S + S; 
		if (*pr->posX - x1 < -distantion * S) *pr->posX = x1 + (-distantion) * S - S; 
		if (*pr->posY - y1 > distantion * S) *pr->posY = y1 + distantion * S + S;
		if (*pr->posY - y1 < -distantion * S) *pr->posY = y1 + (-distantion) * S - S*2;

		//Расчет значений позиции атаки
		int xI = *pr->posX - tiles[0].coord.x; //по клику
		int yI = *pr->posY - tiles[0].coord.y;
		
		for (int j = 0; j < QuanTile; j++) {

			if (tiles[j].coord.x == 0 && tiles[j].coord.y == 0)
				continue; //не учитываем отсутствие тайлов

			tiles[j].coord.x += xI;
			tiles[j].coord.y += yI;
		}
	}
	
	if (FuncAttack == "Direction" && life_attack && !BlockOnce) //функция атаки: по направлению
	{ //система отзеркалирования и паралельности
		
		float rotation = (atan2(*pr->posX - *pr->x, *pr->posY - *pr->y)) * 180 / 3.14; //опеределяем градусы направления

		//изменяем координаты под направление
		for (int j = 0; j < QuanTile; j++)
		{
			if (SAAttack->Tile[j][0] == 0 && SAAttack->Tile[j][1] == 0) {
				tiles[j].coord.x = 0; 
				tiles[j].coord.y = 0; 
				continue;
			}
			
			//берем за основу SAAttack умножаем на -1 и переводим координаты относительно текущей позиции игрока 
			//по стандарту атака должна быть расположена слева
			if (rotation > 40 && rotation < 150) //направо
				tiles[j].coord.x = SAAttack->Tile[j][0] * -1 * SAAttack->SizeTile + *pr->x + pr->Width;
			
			if (rotation < -80) //стандарт (лево)
			{ } 
			
			if (!(rotation < 150 && rotation > -150)) { //наверх
				tiles[j].coord.x = *pr->x - SAAttack->Tile[j][1] * tiles[j].size + pr->Width;
				tiles[j].coord.y = *pr->y - SAAttack->Tile[j][0] * -1 * tiles[j].size;
			} 
			if (rotation < 40 && rotation > -40) { //низ
				tiles[j].coord.x = *pr->x - SAAttack->Tile[j][1] * tiles[j].size + pr->Width;
				tiles[j].coord.y = *pr->y - SAAttack->Tile[j][0] * tiles[j].size + pr->Height;
			}

		}
		
	}

	//DistanceFromOpponent (учет макс. допустимой дистанции)
	if (!BlockOnce && FuncAttack != "ClickPlace") //для ClickPlace свой учет дистанции
	for (int j = 0; j < QuanTile; j++) 
	{
		if (tiles[j].coord.x > *pr->x + distantion * tiles[j].size + pr->Width ||
			tiles[j].coord.x < *pr->x - distantion* tiles[j].size /*+ pr->Width*/ ||

			tiles[j].coord.y > *pr->y + distantion * tiles[j].size + pr->Height ||
			tiles[j].coord.y < *pr->y - distantion* tiles[j].size/* + pr->Height*/) {

			tiles[j].coord.x = 0; //элементы зашедшие за зону макс дистанции будут удалены
			tiles[j].coord.y = 0;
		}
	}
	
	if (FuncAttack != "Left") BlockOnce = true;
}

void attack::vide()
{
	float dx = 0, dy = 0;

	//вид управления атакой (фиксированный путь до цели)
	if (VidE == 1) //метание кинжалов
	{
		//алгоритм построение отрезка/прямой
		if (BlockVidE == 0) //инициализация точки один раз
		{
			saveX_VidE = *pr->x - pr->Width / 2 - *pr->posX; //дистанция от начала до конца
			saveY_VidE = *pr->y - pr->Height / 2 - *pr->posY;
			posX_VidE = *pr->posX; //конечные точки
			posY_VidE = *pr->posY;

			//изменяем координаты атаки на точку начала
			int xI = *pr->x - pr->Width / 2 - tiles[0].coord.x; //по клику
			int yI = *pr->y - tiles[0].coord.y;

			for (int j = 0; j < 20; j++) {
				tiles[j].coord.x += xI;
				tiles[j].coord.y += yI;
			}

			BlockVidE = 1;
		}

		float poX = saveX_VidE; //переведем во float
		float poY = saveY_VidE;

		int nx = 0, ny = 0;
		if (saveX_VidE + posX_VidE < posX_VidE) nx = 1; else nx = -1; //по графику
		if (saveY_VidE + posY_VidE < posY_VidE) ny = 1; else ny = -1;

		if (poX / poY > 1 || poX / poY < -1) { //если x превышает 1 то x = 1 а y < 1

			dx = nx * 3; //высчитываем алгоритм прибавления для равномерной уменьшении дистанции
			dy = poY / poX * nx * 3;
		}
		else { //наоборот
			dx = poX / poY * ny * 3;
			dy = ny * 3;
		}
		//алгоритм выравнивания
		//необходимо чтобы число было не меньше 1
		if (dx <= 1)
			for (int j = 0; j < 20; j++)
				if (dx * j >= 1 || dx * j <= -1) {
					dx = dx * j;
					dy = dy * j;
					break;
				}
		if (dy <= 1)
			for (int j = 0; j < 20; j++)
				if (dy * j >= 1 || dy * j <= -1) {
					dx = dx * j;
					dy = dy * j;
					break;
				}

		tiles[QuanTile - 2].duration = 0.01 + dx / 500 + dy / 500; //регулирование скорости (требуется регулирование)
		if (tiles[QuanTile - 2].duration < 0.01) tiles[QuanTile - 2].duration = 0.02 - tiles[QuanTile - 2].duration;

		dx = rint(dx);
		dy = rint(dy);
	}

	if (VidE == 2) //нож от точки до цели
	{
		//выясним айди противника (тест) это и будет точка конца атаки
		int xx = 0, yy = 0;
		
		for (int j = 0; j < world->getSize(); j++)
			if (world->getEntity(j)->getId() != pr->id) {
				xx = world->getEntity(j)->getPos().x; //pr.posX
				yy = world->getEntity(j)->getPos().y;
				break; 
			} 

		if (BlockVidE == 0) { //один раз
			//DistanceFromOpponent - 420
			//создаем рандомное место по кружности от цели
			int idx = 1;
			saveX_VidE = rand() % 720 - 300;

			if (saveX_VidE < 0) idx = -1;
			saveY_VidE = 420 - saveX_VidE * idx;

			if (rand() % 2 == 1) saveX_VidE *= -1;
			if (rand() % 2 == 0) saveY_VidE *= -1;
			//-----------------------

			int xI = xx + saveX_VidE - tiles[0].coord.x; //по клику
			int yI = yy + saveY_VidE - tiles[0].coord.y;

			for (int j = 0; j < 20; j++) {
				tiles[j].coord.x += xI;
				tiles[j].coord.y += yI;
			}
			BlockVidE = 1;
		}
		
		float poX = xx + saveX_VidE - xx; //дистанция до противника от начала атаки
		float poY = yy + saveY_VidE - yy; //xx - 200 = начало атаки

		int nx = 0, ny = 0; //доп направления
		if (xx + saveX_VidE < xx) nx = 1; else nx = -1; //по графику
		if (yy + saveY_VidE < yy) ny = 1; else ny = -1;

		if (poX / poY > 1 || poX / poY < -1) { //если x превышает 1 то x = 1 а y < 1

			dx = nx * 3;
			dy = poY / poX * nx * 3;
		}
		else { //наоборот
			dx = poX / poY * ny * 3;
			dy = ny * 3;
		}

		//алгоритм выравнивания
		//необходимо чтобы число было не меньше 1
		if (dx <= 1)
			for (int j = 0; j < 20; j++)
				if (dx * j >= 1 || dx * j <= -1) {
					dx = dx * j;
					dy = dy * j;
					break;
				}
		if (dy <= 1)
			for (int j = 0; j < 20; j++)
				if (dy * j >= 1 || dy * j <= -1) {
					dx = dx * j;
					dy = dy * j;
					break;
				}
		
		dx = rint(dx);
		dy = rint(dy);
	} 

	//вид правления атакой  (дистанционный путь до цели)
	//в этом случае нужен один фактор выбора противника - целенаправленный
	if (VidE == 3) //прикольно
	{
		int xx = *pr->posX, yy = *pr->posY;
		if (tiles[0].coord.x < xx) dx = 1; else dx = -1;
		if (tiles[0].coord.y < yy) dy = 1; else dy = -1;
		dx *= 2; //up speed
		dy *= 2;
	}

	//движение атаки по окружности
	if (VidE == 4)
	{ 
		float rotation = 150 * time;
		int radius = 2;

		if (rotation >= 360) //не больше 360
			rotation = 360 * (rotation / 360 - int(rotation / 360));

		//позиция
		if (rotation >= 180) dx = 1; else dx = -1;
		if (rotation >= 0)   dy = -1;
		if (rotation >= 90)  dy = 1;
		if (rotation >= 270) dy = -1; 
		dx *= 3; dy *= 3;
	}

	//движение атаки
	for (int j = 0; j < 20; j++) {
		tiles[j].coord.x += dx;
		tiles[j].coord.y += dy;
	}
}

void attack::setAttribute(GameWorld* world_, View* view_)
{
	world = world_; 
	view = view_; 
}

void attack::inisialization()
{
	//загрузка атаки из CurrentAttack
	QuanTile        = SAAttack->QuanityTile;
	type_attack     = SAAttack->type_attack; //урон в сек
	FuncAttack      = SAAttack->FuncAttack;
	distantion      = SAAttack->distantion;
	timeAttack_VidE = SAAttack->tVide + time;
	VidE            = SAAttack->VidE;
	tiles.clear();

	for (int j = 0; j < QuanTile; j++)
	{
		tiles.push_back(Tile()); //tiles.coord являются измененными относительно аватара игрока
		if (SAAttack->Tile[j][0] != 0) tiles[j].coord.x = SAAttack->Tile[j][0] + *pr->x / SAAttack->SizeTile;
		if (SAAttack->Tile[j][1] != 0) tiles[j].coord.y = SAAttack->Tile[j][1] + *pr->y / SAAttack->SizeTile;

		tiles[j].delay    = SAAttack->delay[j];
		tiles[j].duration = SAAttack->duration[j];
		tiles[j].size     = SAAttack->SizeTile;
	}

	for (int j = 0; j < QuanTile; j++) { //переводим координаты в обычный размер (1x1 pixel)
		tiles[j].coord.x *= SAAttack->SizeTile;
		tiles[j].coord.y *= SAAttack->SizeTile;
	}
}

bool attack::CheckGroup(playerPr* pr, int id)
{
	if (pr->group == NULL) return true;
	for (int j = 0; j < pr->group->vec.size(); j++)
		if (pr->group->vec[j]->getId() == id) return false;
	return true;
}

int attack::getVidE()
{
	return VidE;
}


GameWorld* attack::world = NULL;
BridgeClient* attack::client_br = new BridgeClient;
View* attack::view = new View;

//----------------AttackSystem
AttackSystem::AttackSystem()
{
	animation = new AAforASystem();
	animation->at1 = AnimationSystem->newScructureAnimation();
	animation->at2 = AnimationSystem->newScructureAnimation();
	animation->at3 = AnimationSystem->newScructureAnimation();
	animation->left = AnimationSystem->newScructureAnimation();
	SAt1 = { &pr_, &SAAttack[0], animation->at1 }, //3 особых атаки (speciality attacks)
	SAt2 = { &pr_, &SAAttack[1], animation->at2 },
	SAt3 = { &pr_, &SAAttack[2], animation->at3 },
	Left = { &pr_, &SAAttack[0], animation->left };
}
AttackSystem::AttackSystem(AAforASystem *structure)
{
	if (structure == NULL) return;
	animation = structure;
	SAt1 = { &pr_, &SAAttack[0], structure->at1 }, //3 особых атаки (speciality attacks)
	SAt2 = { &pr_, &SAAttack[1], structure->at2 },
	SAt3 = { &pr_, &SAAttack[2], structure->at3 },
	Left = { &pr_, &SAAttack[0], structure->left };
}

AttackSystem::~AttackSystem()
{
	AnimationSystem->DeleteAnimation(animation->at1);
	AnimationSystem->DeleteAnimation(animation->at2);
	AnimationSystem->DeleteAnimation(animation->at3);
	AnimationSystem->DeleteAnimation(animation->left); 
	delete animation;
}

void AttackSystem::go_attack() //настраивает обьекты Attack под систему 3 атак и обычной атаки
{
	//атака
	if (Left.life_attack || numLeft) { Left.life_attack = true; Left.attack_start(); }
	
	if (SAt1.life_attack || num1) SAt1.go_attack();
	if (SAt2.life_attack || num2) SAt2.go_attack();
	if (SAt3.life_attack || num3) SAt3.go_attack();
	
	//проверка на активность атаки
	if (num1 && !SAt1.life_attack) num1 = false; //отключаем num если атаки не активны
	if (num2 && !SAt2.life_attack) num2 = false;
	if (num3 && !SAt3.life_attack) num3 = false;
	
	if ((numLeft || Left.life_attack) && !Left.life_attack) { //отключаем обычную атаку
		numLeft = false; 
		Left.life_attack = false;
	}
}

void AttackSystem::inisialization()
{
	SAt1 = { &pr_, &SAAttack[0], animation->at1 }; //переинициализируем
	SAt2 = { &pr_, &SAAttack[1], animation->at2 };
	SAt3 = { &pr_, &SAAttack[2], animation->at3 };
	Left = { &pr_, &SAAttack[0], animation->left };
	
	SAt1.inisialization();
	SAt2.inisialization();
	SAt3.inisialization();

	for (int j = 0; j < many.size(); j++)
		many[j].inisialization();
}

void AttackSystem::setNum(int number, bool T)
{
	switch (number)
	{
	case 1: num1 = T; pr_.currentAt = pr_.at1; break;
	case 2: num2 = T; pr_.currentAt = pr_.at2; break;
	case 3: num3 = T; pr_.currentAt = pr_.at3; break;
	case 4: numLeft = T; pr_.currentAt = pr_.normAt; break;
	}
}

void AttackSystem::setPr(playerPr* a)
{
	pr_.x = a->x;
	pr_.y = a->y;
	pr_.posX = a->posX;
	pr_.posY = a->posY;
	pr_.id = a->id;
	pr_.pid = a->pid;
	pr_.Height = a->Height;
	pr_.Width = a->Width;
	pr_.hp = a->hp;
	pr_.maxHp = a->maxHp;
	pr_.currentAt = a->currentAt;
	pr_.normAt = a->normAt;
	pr_.at1 = a->at1;
	pr_.at2 = a->at2;
	pr_.at3 = a->at3;
}

void AttackSystem::setSAAttack(CurrentAttack* t, int number)
{
	if (number < 0 || number >= 3) return;
	SAAttack[number] = *t;
}

bool AttackSystem::getNum(int number) 
{
	switch (number)
	{
	case 1: return num1; 
	case 2: return num2;
	case 3: return num3;
	case 4: return numLeft;
	}
}

playerPr* AttackSystem::getPr()
{
	return &pr_;
}

CurrentAttack* AttackSystem::getSAAttack(int numb)
{
	if (!(numb >= 0 && numb < 3)) return NULL;

	return &SAAttack[numb];
	
}

vector<attack>& AttackSystem::getMany()
{
	return many;
}

AAforASystem* AttackSystem::getAnimation()
{
	return animation;
}

void AttackSystem::addObj(playerPr* pr, CurrentAttack* cr_)
{
	many.push_back(attack(pr, cr_, AnimationSystem->newScructureAnimation()));
}

void AttackSystem::setAnimationSystem(AnimationAttackModule* an)
{
	AnimationSystem = an;
}

AnimationAttackModule* AttackSystem::AnimationSystem = NULL;
