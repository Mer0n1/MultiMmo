#include "Attack.h"
#include "GameWorld.h"
#include "windows.h"
//-----------------------------attacks
attack::attack() //������������� ����� 2 �������� ����������� �� ��������� ������
{
	time = 0; 
	accounting = 0;
	for (int j = 0; j < 20; j++) id_opponents[j] = 0;
}

attack::attack(playerPr* pr_, CurrentAttack* SAAttack_, AnimationAttack* animation_)
{
	pr = pr_;
	SAAttack = SAAttack_;
	animation = animation_;
	
	for (int j = 0; j < 20; j++) id_opponents[j] = 0;
	for (int j = 0; j < 100; j++) {
		x_attack[j] = 0;
		y_attack[j] = 0;
		delay_attack[j] = 0;
		duration_attack[j] = 0;
	}
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
	
	//�������� ����������� � ����������
	if (time > time_attack && !life_attack) {
		clock.restart(); //����� �������� (����� ������������ �� ������ ����� life_attack �� ������� � ����������� ������)
		time = 0;
		time_attack = time + SAAttack->recharge; //�����������
		life_attack = true;

		if (FuncAttack != "ClickPlace") //���� �� ClickPlace �� ��������� ������������ ������
			for (int j = 0; j < QuanTile; j++) {
				if (SAAttack->Tile[j][0] == 0 & SAAttack->Tile[j][1] == 0) continue;
				x_attack[j] = (SAAttack->Tile[j][0]  + *pr->x / SAAttack->SizeTile) * SAAttack->SizeTile;
				y_attack[j] = (SAAttack->Tile[j][1]  + *pr->y / SAAttack->SizeTile) * SAAttack->SizeTile;
			}
		if (type_attack == Do)
			if (VidE == 0) duration_attack[QuanTile - 2] = 0.2; //���� ���������� ����� ������ ���� �����
	}
	
	//������ �����
	if (life_attack) attack_start();
}

void attack::attack_start()
{
	funcAttack();

	if (life_attack)
	{
	    for (int j = 0; j < 20; j++) 
			id_opponents[j] = 1; //������������ ���� ���� �� ������� ��������� ����

		int CurrentAtt = *pr->currentAt; //������� ���� �����
		active = true;

		for (int j = 0; j < QuanTile; j++) {

			if (j == QuanTile - 1) { //�����
				animation->Reset();
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
			
			if (save_j != -1 && !delayBlock) //������ ����� ����������� ����� ��������� delay - ��������
				for (int c = 0; c < QuanTile; c++) {
					animation->intTile_8[c].setScale(SAAttack->SizeTile / 8.f, SAAttack->SizeTile / 8.f); //8
					animation->intTile_8[c].setColor(Color(255, 255, 255, 50)); //������ ��������������
					animation->intTile_8[c].setPosition(x_attack[c], y_attack[c]); //��������� ������� ������ � ����������� � ������
				}

			if (!delayBlock) { //���� ���� ��������
				if (delay_attack[j] != 0 & save_j < j) {
					newtimer = delay_attack[j] + time; 
					save_j = j;
				} //��������� ��������		
				
				if (newtimer < time) {
					if (newtimer != 0) {
						j = save_j; 
						newtimer = 0; 
						save_j = -1;
					}
				} else break;		
			}
			
			if (duration_attack[j] != 0 & save_j < j & time > newtimer) {
				newtimer = duration_attack[j] + time; 
				save_j = j; 
				delayBlock = true; 
			} //��������� �����������������	
			
			if (newtimer > time) { //����������
				if (save_j2 == save_j)
					save_j2 = 1;
			}
			else if (save_j > -1) { //����� ������������� ����������������� ����������
				j = save_j; //���������� ��������
				save_j = -1;
				newtimer = 0;
				delayBlock = false;
			}
			
			//---------------------------------------- //�������� ��������������� 
			
			for (int c = 0; c < world->getSize(); c++)
			{
				int newJ = j;
				if (save_j != -1) newJ = save_j; //���������� �� ���� ������ ������� ���������� duration
				
				for (int jj = 0; jj < newJ; jj++) 
					if (world->getEntity(c)->getPid() != pr->pid && /*gro(pr, world->getEntity(c)->getId()) &&*/ //�� ���� ���� �� �������
						x_attack[jj] + SAAttack->SizeTile >= world->getEntity(c)->getPos().x &&
						x_attack[jj] <= world->getEntity(c)->getPos().x + world->getEntity(c)->getWidth() &&
						//��������� �������� �� ������� ��������� ����� �� ���������
						y_attack[jj] <= world->getEntity(c)->getPos().y + world->getEntity(c)->getHeight() &&
						y_attack[jj] + SAAttack->SizeTile >= world->getEntity(c)->getPos().y)
					{
						if (!CheckGroup(pr, world->getEntity(c)->getId())) continue;
						for (int i = 0; i < 20; i++) {
							if (id_opponents[i] == world->getEntity(c)->getPid()) break; //���� ����� ���� �� ���������

							if (id_opponents[i] == 1) { //��������� ���� ����������� ��� ������
								id_opponents[i] = world->getEntity(c)->getPid(); break;
							}
						}
						break;
					}
			} 
			
			//---------------------------------------- ��� �����
			if (type_attack != Do) accounting = 0; //���� ��� �� Do �� ��������� ������ �������� ����
			
			if (type_attack == dpsa) //���� � ������� 
			{
				if (newtimer != 0) {
					active = false;
					if (time > saveS) {
						saveS = time + 1;
						if (VidE == 0) CurrentAtt = (*pr->currentAt) / duration_attack[save_j];
						if (VidE != 0) CurrentAtt = (*pr->currentAt) / SAAttack->tVide;
						if (*pr->currentAt < 1) *pr->currentAt = 1; //�� ������ 1
						active = true; 
					}
				}
				else active = false;
			}
			
			if (type_attack == ida) //���� � ���������
			{
				if (newtimer != 0) {
					active = false; 
					if (time > saveS) {
						if (VidE == 0) saveS = time + 1.f / (*pr->currentAt / duration_attack[save_j]);
						if (VidE != 0) saveS = time + 1.f / (*pr->currentAt / SAAttack->tVide);
						CurrentAtt = 1; active = true;
					}
				}
				else active = false;
			}

			//��� ����� 
			if (VidE != 0 && j == QuanTile - 2 && save_j == -1)  //���� duration ������� ���������
				if (timeAttack_VidE > time) { //��������� ����� �� ��������� �������
					j = 0; vide(); //������ ���� - 1 ����
					if (duration_attack[18] == 0)
						duration_attack[18] = 0.01;
				}
			//--------------------------------------------------- ��������� �����
			
			if (active) //��� ���������� �����
				for (int c = 0; c < world->getSize(); c++) 
					for (int i = accounting; i < 20; i++)
						if (world->getEntity(c)->getPid() == id_opponents[i]) {
							world->getEntity(c)->updateXp(CurrentAtt); //������� ����
							client_br->MakeDamage(id_opponents[i], CurrentAtt); //���������� ���� �������� �� ������
							accounting = i+1; //��� Do
						}
					
			
			//�������� �����
			if (x_attack[j] != 0 && y_attack[j] != 0) {
				animation->intTile_8[j].setColor(Color(255, 255, 255, 255)); //������� ������������
				animation->intTile_8[j].setScale(SAAttack->SizeTile / 8.f, SAAttack->SizeTile / 8.f); //8
				animation->intTile_8[j].setPosition(x_attack[j], y_attack[j]); //��������� ������� ������ � ����������� � ������
			}
			
			if (newtimer > time) break; //������� ����� ����������� ���� �� ��������� ��� �����������������
		}

	}
}

void attack::funcAttack()
{
	time = clock.getElapsedTime().asSeconds();
	
	if (Left || FuncAttack == "Left") //������� �����
	{
		if (time > time_save) {

			if (pr->id == 99) {

				time_save = time + 0.5;
				int y1 = *pr->y / 32,
					x1 = *pr->x / 32;

				//������ ����������� �����
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
				y_attack[0] = y2 * 32; x_attack[0] = x2 * 32;
				SAAttack->SizeTile = 32; 
				life_attack = true;
			}

			if (pr->id == 8) //������
			{
				//2 ��� ������� ����� ��� ����� 2 ����������� ������ 8 (������ � �����)
				time_save = time + 3;
				int y1 = *pr->y / 32,
					x1 = *pr->x / 32;

				float rotation = (atan2(*pr->posX - view->getCenter().x, *pr->posY - view->getCenter().y)) * 180 / 3.14;

				if (rotation < 0) {
					x_attack[0] = x1; y_attack[0] = y1;
					x_attack[1] = x1 - 1; y_attack[1] = y1;
					x_attack[2] = x1 - 1; y_attack[2] = y1 + 1;
					x_attack[3] = x1 - 1; y_attack[3] = y1 + 2;
					x_attack[4] = x1 - 1; y_attack[4] = y1 + 3;
					x_attack[5] = x1 - 1; y_attack[5] = y1 + 4;
					x_attack[6] = x1; y_attack[6] = y1 + 4;
				}
				else {
					x_attack[0] = x1 + 2; y_attack[0] = y1;
					x_attack[1] = x1 + 3; y_attack[1] = y1;
					x_attack[2] = x1 + 3; y_attack[2] = y1 + 1;
					x_attack[3] = x1 + 3; y_attack[3] = y1 + 2;
					x_attack[4] = x1 + 3; y_attack[4] = y1 + 3;
					x_attack[5] = x1 + 3; y_attack[5] = y1 + 4;
					x_attack[6] = x1 + 2; y_attack[6] = y1 + 4;
				}
				//numberNum = 0;
				SAAttack->SizeTile = 32;
				type_attack = Do;
				life_attack = true;
			}
		}
		else Left = false;
	}

	if (FuncAttack == "line" && life_attack && !BlockOnce) //����� ������ (��� - �����)
	{
		int y1 = (*pr->y + pr->Height / 2) / SAAttack->SizeTile,
			x1 = (*pr->x + pr->Width / 2) / SAAttack->SizeTile; //����� ����� �������
		
		//������ �����������
		int nx = 0, ny = 0; //�����������
		if (*pr->posX > *pr->x) nx = 1; else nx = -1; //�� �������
		if (*pr->posY > *pr->y) ny = 1; else ny = -1;

		int posX = *pr->posX, posY = *pr->posY; //������� � int
		float poX = posX / SAAttack->SizeTile - x1; //������� ����� ������ ������
		float poY = posY / SAAttack->SizeTile - y1; //�� ������ �������

		float itogX = 0, itogY = 0; //������ �������� �������� ���

		for (int j = 0; j < 20; j++) { //x � y �� ������ ��������� 1

			if (poY == 0) continue;

			if (poX / poY > 1 || poX / poY < -1) { //���� x ��������� 1 �� x = 1 � y < 1

				itogX += nx;
				itogY += poY / poX * nx;
			}
			else { //��������
				itogX += poX / poY * ny;
				itogY += ny;
			}
			x_attack[j] = x1 + itogX;
			y_attack[j] = y1 + itogY;
		}

		for (int j = 0; j < QuanTile; j++) {
			x_attack[j] = x_attack[j] * SAAttack->SizeTile;
			y_attack[j] = y_attack[j] * SAAttack->SizeTile;
		}

		//BlockOnce = true;
		//�������� �� ������� ��� ������������� ��������� �� x � y �� �����, � ����������� ����� ��������� 1 : x - �������������� �����
	}

	if (FuncAttack == "ClickPlace" && life_attack && !BlockOnce) //������� �����: �� ����� �����
	{
		//��������� ���������
		int x1 = *pr->x + pr->Width / 2;
		int y1 = *pr->y + pr->Height / 2; 
		int S = 32; 
		
		if (*pr->posX - x1 > distantion * S) *pr->posX = x1 + distantion * S + S; 
		if (*pr->posX - x1 < -distantion * S) *pr->posX = x1 + (-distantion) * S - S; 
		if (*pr->posY - y1 > distantion * S) *pr->posY = y1 + distantion * S + S;
		if (*pr->posY - y1 < -distantion * S) *pr->posY = y1 + (-distantion) * S - S*2;

		//������ �������� ������� �����
		int xI = *pr->posX - x_attack[0]; //�� �����
		int yI = *pr->posY - y_attack[0]; 
		
		for (int j = 0; j < QuanTile; j++) {

			if (x_attack[j] == 0 && x_attack[j] == 0)
				continue; //�� ��������� ���������� ������

			x_attack[j] += xI;
			y_attack[j] += yI;
		}
	}

	if (FuncAttack == "Direction" && life_attack && !BlockOnce) //������� �����: �� �����������
	{ //������� ���������������� � �������������
		
		float rotation = (atan2(*pr->posX - *pr->x, *pr->posY - *pr->y)) * 180 / 3.14; //����������� ������� �����������

		//�������� ���������� ��� �����������
		for (int j = 0; j < QuanTile; j++)
		{
			if (SAAttack->Tile[j][0] == 0 && SAAttack->Tile[j][1] == 0) {
				x_attack[j] = 0; y_attack[j] = 0; continue;
			}
			
			//����� �� ������ SAAttack �������� �� -1 � ��������� ���������� ������������ ������� ������� ������ 
			//�� ��������� ����� ������ ���� ����������� �����
			if (rotation > 40 && rotation < 150) //�������
				x_attack[j] = SAAttack->Tile[j][0] * -1 * SAAttack->SizeTile + *pr->x + pr->Width;
			
			if (rotation < -80) //�������� (����)
			{ } 
			
			if (!(rotation < 150 && rotation > -150)) { //������
				x_attack[j] = *pr->x - SAAttack->Tile[j][1] * SAAttack->SizeTile + pr->Width;
				y_attack[j] = *pr->y - SAAttack->Tile[j][0] * -1 * SAAttack->SizeTile;
			} 
			if (rotation < 40 && rotation > -40) { //���
				x_attack[j] = *pr->x - SAAttack->Tile[j][1] * SAAttack->SizeTile + pr->Width;
				y_attack[j] = *pr->y - SAAttack->Tile[j][0] * SAAttack->SizeTile + pr->Height;
			}

		}
	}

	//DistanceFromOpponent (���� ����. ���������� ���������)
	if (!BlockOnce && FuncAttack != "ClickPlace") //��� ClickPlace ���� ���� ���������
	for (int j = 0; j < QuanTile; j++) 
	{
		if (x_attack[j] > *pr->x + distantion * SAAttack->SizeTile + pr->Width || 
			x_attack[j] < *pr->x - distantion* SAAttack->SizeTile /*+ pr->Width*/ ||

			y_attack[j] > *pr->y + distantion * SAAttack->SizeTile + pr->Height ||
			y_attack[j] < *pr->y - distantion* SAAttack->SizeTile/* + pr->Height*/) {

			x_attack[j] = 0; //�������� �������� �� ���� ���� ��������� ����� �������
			y_attack[j] = 0; 
		}
	}
	
	if (FuncAttack != "Left") BlockOnce = true;
}

void attack::vide()
{
	float dx = 0, dy = 0;

	//��� ���������� ������ (������������� ���� �� ����)
	if (VidE == 1) //������� ��������
	{
		//�������� ���������� �������/������
		if (BlockVidE == 0) //������������� ����� ���� ���
		{
			saveX_VidE = *pr->x - pr->Width / 2 - *pr->posX; //��������� �� ������ �� �����
			saveY_VidE = *pr->y - pr->Height / 2 - *pr->posY;
			posX_VidE = *pr->posX; //�������� �����
			posY_VidE = *pr->posY;

			//�������� ���������� ����� �� ����� ������
			int xI = *pr->x - pr->Width / 2 - x_attack[0]; //�� �����
			int yI = *pr->y - y_attack[0];

			for (int j = 0; j < 20; j++) {
				x_attack[j] += xI;
				y_attack[j] += yI;
			}

			BlockVidE = 1;
		}

		float poX = saveX_VidE; //��������� �� float
		float poY = saveY_VidE;

		int nx = 0, ny = 0;
		if (saveX_VidE + posX_VidE < posX_VidE) nx = 1; else nx = -1; //�� �������
		if (saveY_VidE + posY_VidE < posY_VidE) ny = 1; else ny = -1;

		if (poX / poY > 1 || poX / poY < -1) { //���� x ��������� 1 �� x = 1 � y < 1

			dx = nx * 3; //����������� �������� ����������� ��� ����������� ���������� ���������
			dy = poY / poX * nx * 3;
		}
		else { //��������
			dx = poX / poY * ny * 3;
			dy = ny * 3;
		}
		//�������� ������������
		//���������� ����� ����� ���� �� ������ 1
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

		duration_attack[18] = 0.01 + dx / 500 + dy / 500; //������������� �������� (��������� �������������)
		if (duration_attack[18] < 0.01) duration_attack[18] = 0.02 - duration_attack[18];

		dx = rint(dx);
		dy = rint(dy);
	}


	if (VidE == 2) //��� �� ����� �� ����
	{
		//������� ���� ���������� (����) ��� � ����� ����� ����� �����
		int xx = 0, yy = 0;
		
		for (int j = 0; j < world->getSize(); j++)
			if (world->getEntity(j)->getId() != pr->id) {
				xx = world->getEntity(j)->getPos().x; //pr.posX
				yy = world->getEntity(j)->getPos().y;
				break; 
			} 

		if (BlockVidE == 0) { //���� ���
			//DistanceFromOpponent - 420
			//������� ��������� ����� �� ��������� �� ����
			int idx = 1;
			saveX_VidE = rand() % 720 - 300;

			if (saveX_VidE < 0) idx = -1;
			saveY_VidE = 420 - saveX_VidE * idx;

			if (rand() % 2 == 1) saveX_VidE *= -1;
			if (rand() % 2 == 0) saveY_VidE *= -1;
			//-----------------------

			int xI = xx + saveX_VidE - x_attack[0]; //�� �����
			int yI = yy + saveY_VidE - y_attack[0];

			for (int j = 0; j < 20; j++) {
				x_attack[j] += xI;
				y_attack[j] += yI;
			}
			BlockVidE = 1;
		}
		
		float poX = xx + saveX_VidE - xx; //��������� �� ���������� �� ������ �����
		float poY = yy + saveY_VidE - yy; //xx - 200 = ������ �����

		int nx = 0, ny = 0; //��� �����������
		if (xx + saveX_VidE < xx) nx = 1; else nx = -1; //�� �������
		if (yy + saveY_VidE < yy) ny = 1; else ny = -1;

		if (poX / poY > 1 || poX / poY < -1) { //���� x ��������� 1 �� x = 1 � y < 1

			dx = nx * 3;
			dy = poY / poX * nx * 3;
		}
		else { //��������
			dx = poX / poY * ny * 3;
			dy = ny * 3;
		}

		//�������� ������������
		//���������� ����� ����� ���� �� ������ 1
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

	//��� ��������� ������  (������������� ���� �� ����)
	//� ���� ������ ����� ���� ������ ������ ���������� - ����������������
	if (VidE == 3) //���������
	{
		int xx = *pr->posX, yy = *pr->posY;
		if (x_attack[0] < xx) dx = 1; else dx = -1;
		if (y_attack[0] < yy) dy = 1; else dy = -1; 
		dx *= 2; //up speed
		dy *= 2;
	}

	//����� ����� 2 ��� ���� (����� ������������ � update �������)
	if (VidE == 4)
	{ 
		float rotation = 150 * time;
		int radius = 2;

		if (rotation >= 360) //�� ������ 360
			rotation = 360 * (rotation / 360 - int(rotation / 360));

		//�������
		if (rotation >= 180) dx = 1; else dx = -1;
		if (rotation >= 0) dy = -1;
		if (rotation >= 90) dy = 1;
		if (rotation >= 270) dy = -1; 
		dx *= 3; dy *= 3;
	}

	//�������� �����
	for (int j = 0; j < 20; j++) {
		x_attack[j] = x_attack[j] + dx;
		y_attack[j] = y_attack[j] + dy;
	}
}

void attack::setAttribute(GameWorld* world_, View* view_)
{
	world = world_; 
	view = view_; 
}

void attack::inisialization()
{
	//�������� ����� �� CurrentAttack
	for (int j = 0; j < 100; j++) {
		
		if (SAAttack->Tile[j][0] == 0 &
			SAAttack->Tile[j][1] == 0) continue;
		
		x_attack[j] = SAAttack->Tile[j][0]  + *pr->x / SAAttack->SizeTile;
		y_attack[j] = SAAttack->Tile[j][1]  + *pr->y / SAAttack->SizeTile;
		
		delay_attack[j] = SAAttack->delay[j];
		duration_attack[j] = SAAttack->duration[j];
	}
	
	if (SAAttack->FuncAttack == "line") //��������� ������� ����� (�������)
		for (int j = 0; j < 100; j++) {
			delay_attack[j] = SAAttack->delay[j];
			duration_attack[j] = SAAttack->duration[j];
		}

	type_attack = SAAttack->type_attack; //���� � ���
	FuncAttack = SAAttack->FuncAttack;
	distantion = SAAttack->distantion;
	QuanTile = SAAttack->QuanityTile;
	timeAttack_VidE = SAAttack->tVide + time;
	VidE = SAAttack->VidE;

	for (int j = 0; j < QuanTile; j++) { //��������� ���������� � ������� ������ (���� ��� �� �����)
		x_attack[j] = x_attack[j] * SAAttack->SizeTile;
		y_attack[j] = y_attack[j] * SAAttack->SizeTile; 
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

bool attack::getLife()
{
	return life_attack;
}

bool* attack::getLeft()
{
	return &Left;
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
	SAt1 = { &pr_, &SAAttack[0], animation->at1 }, //3 ������ ����� (speciality attacks)
	SAt2 = { &pr_, &SAAttack[1], animation->at2 },
	SAt3 = { &pr_, &SAAttack[2], animation->at3 },
	Left = { &pr_, &SAAttack[0], animation->left };
}
AttackSystem::AttackSystem(AAforASystem *structure)
{
	animation = structure;
	SAt1 = { &pr_, &SAAttack[0], structure->at1 }, //3 ������ ����� (speciality attacks)
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

void AttackSystem::go_attack() //����������� ������� Attack ��� ������� 3 ���� � ������� �����
{
	//�����
	if (Left.getLife() || numLeft) { *Left.getLeft() = true; Left.attack_start(); }
	
	if (SAt1.getLife() || num1) SAt1.go_attack();
	if (SAt2.getLife() || num2) SAt2.go_attack();
	if (SAt3.getLife() || num3) SAt3.go_attack();
	
	//�������� �� ���������� �����
	if (num1 && !SAt1.getLife()) num1 = false; //��������� num ���� ����� �� �������
	if (num2 && !SAt2.getLife()) num2 = false;
	if (num3 && !SAt3.getLife()) num3 = false;
	
	if ((numLeft || Left.getLeft()) && !Left.getLife()) { //��������� ������� �����
		numLeft = false; 
		*Left.getLeft() = false;
	}
}

void AttackSystem::inisialization()
{
	SAt1 = { &pr_, &SAAttack[0], animation->at1 }; //������������������
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
	pr_.xp = a->xp;
	pr_.maxXp = a->maxXp;
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
